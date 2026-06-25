#include <stdio.h>
#include <string.h>
#include <windows.h>

// 프로토타입
int  addTask(char text[][100], int day[], int done[], int count);
int  checkTask(int done[], int count);
int  deleteTask(char text[][100], int day[], int done[], int count);
void showPlanner(int month, int week, char text[][100], int day[], int done[], int count, char dayname[][8]);
void makePath(int m, int w, char path[]);                                                                       // 추가
int  listFiles(int foundM[], int foundW[]);                                                                     // 추가
int  loadFile(char path[], char text[][100], int day[], int done[], char dayname[][8]);                         // 추가
int  saveFile(char path[], char text[][100], int day[], int done[], int count, char dayname[][8]);              // 추가
void bigView(int month, int week, char text[][100], int day[], int done[], int count, char dayname[][8]);

int main(void)
{   
    //저장된 플래너 불러오거나(1~n), 새로운 플래너 만들거나(0), 프로그램 종료하기(-1) 
    char dayname[7][8] = {"월", "화", "수", "목", "금", "토", "일"};
    while (1){
        int foundM[60], foundW[60];
        int foundCount, choice;
        foundCount = listFiles(foundM, foundW);
        printf("\n========== 주간 플래너 ==========\n");
        if (foundCount == 0)
            printf("저장된 플래너가 없습니다.\n");
        else {
            printf("[ 저장된 플래너 ]\n");
            for (int i = 0; i < foundCount; i++){
                printf("  %d. %d월%d주차\n", i + 1, foundM[i], foundW[i]);
            }
        }
        printf("---------------------------------\n");
        printf("번호 = 불러오기  /  0 = 새로 만들기  /  -1 = 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        if (choice == -1){
            printf("프로그램을 종료합니다.\n");
            break;
        }
        char text[100][100];   // 할일 내용 (2차원 배열)
        int  day[100];         // 요일 0~6
        int  done[100];        // 완료 0/1
        int  count = 0;        // 할일 개수
        char path[40];         // 파일 경로
        int  month, week, menu;
        if (choice == 0){
            while(1){          //새로 민들기
                printf("몇 월 플래너인가요? ");
                scanf("%d", &month);
                printf("몇 주차인가요? ");
                scanf("%d", &week);
                if((month > 12 || month < 1) || (week > 5 || week < 1)) printf("잘못된 입력입니다. 다시 입력해 주세요.\n");
                else break;
            }
            makePath(month, week, path);
            count = 0;
        }
        else if (choice >= 1 && choice <= foundCount){
            month = foundM[choice - 1];
            week  = foundW[choice - 1];
            makePath(month, week, path);
            count = loadFile(path, text, day, done, dayname);
            system("cls");
        }
        else {
            printf("잘못된 입력입니다.\n");
            continue;
        }
        while (1){            //매뉴 (1: 추가 2: 체크 3: 삭제 4: 크게보기 0: 저장 및 돌아가기)
            showPlanner(month, week, text, day, done, count, dayname);
            printf("1.할일추가  2.완료체크  3.삭제  4.크게보기  0.저장 후 목록으로\n");
            printf("선택: ");
            scanf("%d", &menu);
            if (menu == 1) count = addTask(text, day, done, count);
            else if (menu == 2) checkTask(done, count);
            else if (menu == 3) count = deleteTask(text, day, done, count);
            else if (menu == 4) bigView(month, week, text, day, done, count, dayname);
            else if (menu == 0){
                saveFile(path, text, day, done, count, dayname);
                system("cls");
                break;
            }
            else printf("잘못된 입력입니다.\n");
        }
    }
    return 0;
}

int addTask(char text[][100], int day[], int done[], int count) //할일 추가
{
    int sel, len, pos;
    char buf[100];
    printf("요일 (1:월 2:화 3:수 4:목 5:금 6:토 7:일): ");
    scanf("%d", &sel);
    getchar();
    if (sel < 1 || sel > 7){
        system("cls");
        printf("잘못된 요일입니다.\n");
        return count;
    }
    sel = sel - 1;
    printf("할일 내용: ");
    fgets(buf, 100, stdin);
    len = strlen(buf);
    buf[strcspn(buf,"\n")]='\0';
    pos = count;
    for (int i = 0; i < count; i++){
        if (day[i] > sel) {
            pos = i;
            break;
        }
    }
    for (int i = count; i > pos; i--){
        strcpy(text[i], text[i - 1]);
        day[i]  = day[i - 1];
        done[i] = done[i - 1];
    }
    strcpy(text[pos], buf);
    day[pos]  = sel;
    done[pos] = 0;
    count++;
    system("cls");
    printf(">> 추가되었습니다.\n");
    return count;
}

int checkTask(int done[], int count) //할일 체크
{
    int num;
    printf("완료/취소할 번호: ");
    scanf("%d", &num);
    if (num < 1 || num > count){
        system("cls");
        printf("잘못된 번호입니다.\n");
        return 0;
    }
    done[num - 1] = (done[num - 1] == 0) ? 1 : 0;
    system("cls");
    printf(">> 변경되었습니다.\n");
    return 1;
}

int deleteTask(char text[][100], int day[], int done[], int count) //할일 삭제
{
    int num;
    printf("삭제할 번호: ");
    scanf("%d", &num);
    if (num < 1 || num > count){
        system("cls");
        printf("잘못된 번호입니다.\n");
        return count;
    }
    for (int i = num - 1; i < count - 1; i++){
        strcpy(text[i], text[i + 1]);
        day[i]  = day[i + 1];
        done[i] = done[i + 1];
    }
    count--;
    system("cls");
    printf(">> 삭제했습니다.\n");
    return count;
}

void showPlanner(int month, int week, char text[][100], int day[], int done[], int count, char dayname[][8]) //화면 출력
{
    printf("\n========== %d월 %d주차 플래너 ==========\n", month, week);
    for (int d = 0; d < 7; d++){
        int empty = 1;
        printf("[%s요일]\n", dayname[d]);
        for (int i = 0; i < count; i++)
            if (day[i] == d){
                char mark = (done[i] == 1) ? 'V' : ' ';
                printf("   %d. [%c] %s\n", i + 1, mark, text[i]);
                empty = 0;
            }
        if (empty == 1) printf("   (없음)\n");
    }
    printf("=======================================\n");
}

void makePath(int m, int w, char path[]) //파일 만들 경로 생성
{
    int i;
    strcpy(path, "scheduals/");
    i = strlen(path);
    if (m >= 10){ path[i++] = '0' + m / 10; path[i++] = '0' + m % 10; }
    else        { path[i++] = '0' + m; }
    path[i] = '\0';
    strcat(path, "월");
    i = strlen(path);
    if (w >= 10){ path[i++] = '0' + w / 10; path[i++] = '0' + w % 10; }
    else        { path[i++] = '0' + w; }
    path[i] = '\0';
    strcat(path, "주차.csv");
}

int listFiles(int foundM[], int foundW[]) //윌,주차 배열에 담고 저장된 개수 return
{
    int count = 0;
    char path[40];
    FILE *fp;
    for (int m = 1; m <= 12; m++)
        for (int w = 1; w <= 5; w++){
            makePath(m, w, path);
            fp = fopen(path, "r");
            if (fp != NULL){
                fclose(fp);
                foundM[count] = m;
                foundW[count] = w;
                count++;
            }
        }
    return count;
}

int loadFile(char path[], char text[][100], int day[], int done[], char dayname[][8]) //제목 버리고 요일,완료여부,할일 한줄 씩 읽는다, 불러온 개수 return
{
    FILE *fp;
    char header[200], dayStr[10];
    int  doneNum, count = 0, d;
    fp = fopen(path, "r");
    if (fp == NULL) return 0;
    fgets(header, 200, fp);   // 제목줄 버리기
    while (fscanf(fp, " %[^,],%d,%[^\n]", dayStr, &doneNum, text[count]) == 3){
        d = -1;
        for (int t = 0; t < 7; t++)
            if (strcmp(dayStr, dayname[t]) == 0) d = t;
        if (d == -1) continue;
        day[count]  = d;
        done[count] = doneNum;
        count++;
    }
    fclose(fp);
    return count;
}

int saveFile(char path[], char text[][100], int day[], int done[], int count, char dayname[][8]) //저장할 파일 생성
{
    FILE *fp;
    fp = fopen(path, "w");
    if (fp == NULL){
        printf("저장 실패! scheduals 폴더가 있는지 확인하세요.\n");
        return 0;
    }
    fprintf(fp, "요일,완료,할일\n");
    for (int i = 0; i < count; i++)
        fprintf(fp, "%s,%d,%s\n", dayname[day[i]], done[i], text[i]);
    fclose(fp);
    return 1;
}

void bigView(int month, int week, char text[][100], int day[], int done[], int count, char dayname[][8]) // 큰 화면 출력
{
    int back;
    while (1){
        system("cls");
        printf("\n  ==================================================\n");
        printf("               %d월 %d주차 플래너\n", month, week);
        printf("  ==================================================\n");
        for (int d = 0; d < 7; d++){
            int empty = 1;
            printf("  ┌─────────────────────────────────────────────\n");
            printf("  │  [ %s요일 ]\n", dayname[d]);
            printf("  ├─────────────────────────────────────────────\n");
            for (int i = 0; i < count; i++){
                if (day[i] == d){
                    char mark = (done[i] == 1) ? 'V' : ' ';
                    printf("  │      [%c] %s\n",mark, text[i]);
                    empty = 0;
                }
            }
            if (empty == 1) printf("  │     (비어 있음)\n");
            printf("  │\n");
            printf("  └─────────────────────────────────────────────\n");
        }
        printf("\n  0 = 돌아가기 / 선택: ");
        scanf("%d", &back);
        if (back == 0){
            system("cls");
            break;
        }
    }
}
#include <stdio.h>
#include <malloc.h>
#include <string.h>
//#include "book.h"
//
typedef struct Book{
	char Book_name[30];
	char Book_company[30];
	char Book_ISBN[20];
	char Book_author[20];
	char Book_borrow; 
	struct Book *next;
	
}Book;

Book *Book_head;
Book *Book_tmp; //맨 끝 책 위치 
Book *ttmp;
Book *ttmp_back;

//
void bk_init(){                          // book 리스트 맨 처음부분 만드는 함수 

FILE* fp=fopen("bk.txt","w");
	Book_head=(Book *)malloc(sizeof(Book));
	
	Book_head->next=NULL;
	Book_tmp=Book_head;	
	fclose(fp);
}

void Load_list_to_file();

int Admin_mode(){                      //관리자 모드 출력 함수 ,선택 수 리턴 
	int choice;
	printf("[관리자 모드]\n");
	printf("1. 도서 등록\n");
	printf("2. 도서 삭제\n");
	printf("3. 도서 대여\n");
	printf("4. 도서 반납\n");
	printf("5. 도서 검색\n");
	printf("6. 회원 목록\n");
	printf("7. 로그아웃\n");
	printf("8.프로그램 종료\n");

	scanf("%d",&choice); 
	return choice;
}

 
 
void add_book(){                 //도서 등록 함수 
	Book *p=(Book *)malloc(sizeof(Book));
	
	printf("[ 도서 등록 ]\n");
	printf("도서명:");
	scanf("%s",p->Book_name);
	printf("출판사:");
	scanf("%s",p->Book_company);
	printf("저자명:");
	scanf("%s",p->Book_author);
	printf("ISBN:");
	scanf("%s",p->Book_ISBN);
	getchar();
	printf("대출가능 여부(y/n):");
	scanf("%c",&p->Book_borrow);
	getchar();
	while(p->Book_borrow!='y'&&p->Book_borrow!='n'){
		printf("'y'와 'n'중 하나를 입력해 주십시오 :");
		scanf("%c",&p->Book_borrow);
		getchar();
		
	} 
	p->next=NULL;
	
	Book_tmp->next=p;
	Book_tmp=Book_tmp->next;

	/*FILE* fp=fopen("bk.txt","w");
	fprintf(fp,"%s %s %s %s %c\n",p->Book_name,p->Book_company,p->Book_author,p->Book_ISBN,p->Book_borrow);
	fclose(fp);
	*/
}

void remove_book(){              // 도서 삭제 함수 
	char name[30];
	printf("도서명:");
	scanf("%s",name);
	ttmp=Book_head->next;
	ttmp_back=Book_head;
	while(ttmp!=NULL&&strcmp(ttmp->Book_name,name)!=0){
		ttmp_back=ttmp;
		ttmp=ttmp->next;
	}
	if(ttmp==NULL){
		printf("%s란 도서를 찾을 수 없습니다.\n",name);
	}else{
		ttmp_back->next=ttmp->next;
		free(ttmp);
		printf("삭제가 완료되었습니다.\n");
	}
}

void borrow_book(){        //도서 대여 함수  수정 필요 
char student[10];
char ISBN[20]; 
	printf("[도서대여]\n");
	printf("학번 :");
	
	scanf("%s",student);
	printf("ISBN :"); 
	 scanf("%s",ISBN);
	 //학생 리스트 이용해서...(학번에 맞는 학생 있으면 대출해줌(학생 목록에 책도 추가해준다) 
	 ttmp=Book_head->next;
	 while(ttmp!=NULL){
	 	if(strcmp(ttmp->Book_ISBN,ISBN)==0){
	 		if(ttmp->Book_borrow=='n'){
	 			printf("이미 대출된 도서입니다.\n");
	 			break;
			 }else{
			 	printf("'%s'책이 대출되었습니다.\n",ttmp->Book_name);
			 	ttmp->Book_borrow='n';
			 	Load_list_to_file();
			 	break;
			 }
			 
		 }
		 ttmp=ttmp->next; 
	 }
	 if(ttmp==NULL){
	 	printf("존재하지 않는 도서입니다.\n");   //오류.. 항상 출력됨 
	 }
	
} 

void return_book(){      //도서 반납 함수 
	
} 

void Load_list_to_file(){   //리스트에서 파일로 입력해주는 함수 
	FILE* fp= fopen("bk.txt","w");
	ttmp=Book_head->next;
	while(ttmp!=NULL){
		fprintf(fp,"%s %s %s %s %c",ttmp->Book_name,ttmp->Book_company,ttmp->Book_author,ttmp->Book_ISBN,ttmp->Book_borrow);
		ttmp=Book_tmp->next;
	}
	fclose(fp);
} 


void Book_load(){                
	char name[30];
	char company[30];
	char ISBN[20];
	char author[20];
	char borrow;
	
	
	FILE* fp=fopen("bk.txt","r");
	int ret=0;

	while(1){
	
		Book *q=(Book *)malloc(sizeof(Book));
		q->next=NULL;
		
		ret=fscanf(fp,"%s %s %s %s %c",name,company,author,ISBN,&borrow);
		
		printf("%s %s %s %s %c",name,company,author,ISBN,borrow);
		
		strcpy(q->Book_name,name);
		strcpy(q->Book_company,company);
		strcpy(q->Book_ISBN,ISBN);
		strcpy(q->Book_author,author);
		
		q->Book_borrow=borrow;
		
		Book_tmp->next=q;
		Book_tmp=q;
		
		if(ret==EOF)
		break;
		
	}
	fclose(fp);
} 

int Find_book(){        //[도서검색]창  ,번호 리턴 
	int num;
	
	printf("[도서검색]\n");
	printf("1. 도서명 검색\n");
	printf("2. 출판사 검색\n");
	printf("3. ISBN 검색\n");
	printf("4. 저자명 검색\n");
	printf("5. 전체 검색\n");
	printf("6. 이전 메뉴\n");
	
	scanf("%d",&num);
	 
	if(num==1){
		
	}else if(num==2){
		
	}else if(num==3){
		
	}else if(num==4){
		
	}else if(num==5){
		
	}else if(num==6){
		
	}else{
		printf("1~6사이의 수를 입력해 주십시오\n");
	}
	
} 
void Book_information(Book *node){      //노드 주소를 받아 정보를 다 출력해주는 함수 
	
	printf("도서명 : %s\n",node->Book_name);
	printf("출판사 : %s\n",node->Book_company);
	printf("ISBN : %s\n",node->Book_ISBN);
	printf("저자명 : %s\n",node->Book_author);
	printf("대출여부 : %c\n",node->Book_borrow);
	
}


void Search_book_name(){         //도서명 검색 함수 
	char name[30];
	printf("책이름: ");
	scanf("%s",name);
	ttmp=Book_head->next;
	
	while(ttmp!=NULL&&strcmp(ttmp->Book_name,name)!=0){
		ttmp_back=ttmp;
		ttmp=ttmp->next;
	}
	if(ttmp==NULL){
		printf("%s란 도서를 찾을 수 없습니다.\n",name);
	}else{
		Book_information(ttmp);
	}
	 
} 
void books(){
	ttmp=Book_head->next;
	while(ttmp!=NULL){
		printf("%s %s %s %s %c\n",ttmp->Book_name,ttmp->Book_company,ttmp->Book_author,ttmp->Book_ISBN,ttmp->Book_borrow);
		ttmp=ttmp->next;
			
	}

}


int main(){
	
	bk_init();
	
	while(1){
	
	int c_num=0;
	c_num=Admin_mode();           ///문자 입력시 무한수열 이유...? 
		
	if(c_num==1){
		add_book();
		Load_list_to_file();
	}else if(c_num==2){
		remove_book();
		Load_list_to_file();
	}else if(c_num==3){
		borrow_book();
		Load_list_to_file();
	}else if(c_num==4){
		
	}else if(c_num==5){
		
	}else if(c_num==6){
		
	}else if(c_num==7){
		
	}else if(c_num==8){
		return 0; 
	}else{
	  printf("1부터 8까지의 수 중 하나를 입력해 주십시오.\n");
	}
		
	
	
}

	return 0;
}

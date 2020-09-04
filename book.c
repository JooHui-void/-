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
Book *Book_tmp; //�� �� å ��ġ 
Book *ttmp;
Book *ttmp_back;

//
void bk_init(){                          // book ����Ʈ �� ó���κ� ����� �Լ� 

FILE* fp=fopen("bk.txt","w");
	Book_head=(Book *)malloc(sizeof(Book));
	
	Book_head->next=NULL;
	Book_tmp=Book_head;	
	fclose(fp);
}

void Load_list_to_file();

int Admin_mode(){                      //������ ��� ��� �Լ� ,���� �� ���� 
	int choice;
	printf("[������ ���]\n");
	printf("1. ���� ���\n");
	printf("2. ���� ����\n");
	printf("3. ���� �뿩\n");
	printf("4. ���� �ݳ�\n");
	printf("5. ���� �˻�\n");
	printf("6. ȸ�� ���\n");
	printf("7. �α׾ƿ�\n");
	printf("8.���α׷� ����\n");

	scanf("%d",&choice); 
	return choice;
}

 
 
void add_book(){                 //���� ��� �Լ� 
	Book *p=(Book *)malloc(sizeof(Book));
	
	printf("[ ���� ��� ]\n");
	printf("������:");
	scanf("%s",p->Book_name);
	printf("���ǻ�:");
	scanf("%s",p->Book_company);
	printf("���ڸ�:");
	scanf("%s",p->Book_author);
	printf("ISBN:");
	scanf("%s",p->Book_ISBN);
	getchar();
	printf("���Ⱑ�� ����(y/n):");
	scanf("%c",&p->Book_borrow);
	getchar();
	while(p->Book_borrow!='y'&&p->Book_borrow!='n'){
		printf("'y'�� 'n'�� �ϳ��� �Է��� �ֽʽÿ� :");
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

void remove_book(){              // ���� ���� �Լ� 
	char name[30];
	printf("������:");
	scanf("%s",name);
	ttmp=Book_head->next;
	ttmp_back=Book_head;
	while(ttmp!=NULL&&strcmp(ttmp->Book_name,name)!=0){
		ttmp_back=ttmp;
		ttmp=ttmp->next;
	}
	if(ttmp==NULL){
		printf("%s�� ������ ã�� �� �����ϴ�.\n",name);
	}else{
		ttmp_back->next=ttmp->next;
		free(ttmp);
		printf("������ �Ϸ�Ǿ����ϴ�.\n");
	}
}

void borrow_book(){        //���� �뿩 �Լ�  ���� �ʿ� 
char student[10];
char ISBN[20]; 
	printf("[�����뿩]\n");
	printf("�й� :");
	
	scanf("%s",student);
	printf("ISBN :"); 
	 scanf("%s",ISBN);
	 //�л� ����Ʈ �̿��ؼ�...(�й��� �´� �л� ������ ��������(�л� ��Ͽ� å�� �߰����ش�) 
	 ttmp=Book_head->next;
	 while(ttmp!=NULL){
	 	if(strcmp(ttmp->Book_ISBN,ISBN)==0){
	 		if(ttmp->Book_borrow=='n'){
	 			printf("�̹� ����� �����Դϴ�.\n");
	 			break;
			 }else{
			 	printf("'%s'å�� ����Ǿ����ϴ�.\n",ttmp->Book_name);
			 	ttmp->Book_borrow='n';
			 	Load_list_to_file();
			 	break;
			 }
			 
		 }
		 ttmp=ttmp->next; 
	 }
	 if(ttmp==NULL){
	 	printf("�������� �ʴ� �����Դϴ�.\n");   //����.. �׻� ��µ� 
	 }
	
} 

void return_book(){      //���� �ݳ� �Լ� 
	
} 

void Load_list_to_file(){   //����Ʈ���� ���Ϸ� �Է����ִ� �Լ� 
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

int Find_book(){        //[�����˻�]â  ,��ȣ ���� 
	int num;
	
	printf("[�����˻�]\n");
	printf("1. ������ �˻�\n");
	printf("2. ���ǻ� �˻�\n");
	printf("3. ISBN �˻�\n");
	printf("4. ���ڸ� �˻�\n");
	printf("5. ��ü �˻�\n");
	printf("6. ���� �޴�\n");
	
	scanf("%d",&num);
	 
	if(num==1){
		
	}else if(num==2){
		
	}else if(num==3){
		
	}else if(num==4){
		
	}else if(num==5){
		
	}else if(num==6){
		
	}else{
		printf("1~6������ ���� �Է��� �ֽʽÿ�\n");
	}
	
} 
void Book_information(Book *node){      //��� �ּҸ� �޾� ������ �� ������ִ� �Լ� 
	
	printf("������ : %s\n",node->Book_name);
	printf("���ǻ� : %s\n",node->Book_company);
	printf("ISBN : %s\n",node->Book_ISBN);
	printf("���ڸ� : %s\n",node->Book_author);
	printf("���⿩�� : %c\n",node->Book_borrow);
	
}


void Search_book_name(){         //������ �˻� �Լ� 
	char name[30];
	printf("å�̸�: ");
	scanf("%s",name);
	ttmp=Book_head->next;
	
	while(ttmp!=NULL&&strcmp(ttmp->Book_name,name)!=0){
		ttmp_back=ttmp;
		ttmp=ttmp->next;
	}
	if(ttmp==NULL){
		printf("%s�� ������ ã�� �� �����ϴ�.\n",name);
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
	c_num=Admin_mode();           ///���� �Է½� ���Ѽ��� ����...? 
		
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
	  printf("1���� 8������ �� �� �ϳ��� �Է��� �ֽʽÿ�.\n");
	}
		
	
	
}

	return 0;
}

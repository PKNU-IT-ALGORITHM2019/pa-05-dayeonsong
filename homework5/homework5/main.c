#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct{
	char word[30];
	char word_class[30];
	char explanation[1000];
}Word;//단어정보저장할 틀
typedef struct tree{
	struct tree*left;
	struct tree*right;
	struct tree*p;
	Word*key;
}Tree;//이진검색트리노드저장할 틀

int count;//단어개수셈
Word*dict[200000];//여러개의 단어정보담을 배열
void insert_BST(Tree**root,Word*key);
Tree* find_word(Tree*x,char str[]);
Tree* delete_word(Tree**root,Tree*node);
Tree* successor(Tree*node);
Tree* find_min(Tree*root);
int deleteall(Tree**root);

int main(void)
{
	FILE*fp=fopen("shuffled_dict.txt","rt");
	Tree*root=NULL;
	char*token;
	char command[30];
	while(!feof(fp)){
		char str[1500];
		fgets(str,sizeof(str),fp);
		dict[count]=(Word*)malloc(sizeof(Word));//단어정보담을 배열에 하나씩 생성
		token=strtok(str,"()");
		strcpy(dict[count]->word,token);
//		dict[count]->word=strtok(str,"()"); char*형식이아니라 할 수 없음
		dict[count]->word[strlen(dict[count]->word)-1]=NULL;//공백제거
		token=strtok(NULL,"()");
		strcpy(dict[count]->word_class,token);
		token=strtok(NULL,"()");
		if(token==NULL){//품사정보없을시
			char str[1500];
			strcpy(str,dict[count]->word_class);
			strcpy(dict[count]->explanation,str);
			strcpy(dict[count]->word_class,NULL);}
		else
			strcpy(dict[count]->explanation,token);
		dict[count]->explanation[strlen(dict[count]->explanation)-1]=NULL;//엔터제거
		insert_BST(&root,dict[count]);//이진검색트리만들기
	}
	while(1){
		printf("$ ");
		scanf("%s",command);
		if(strcmp(command,"size")==0)
			printf("%d \n",count);
		else if(strcmp(command,"find")==0){
			char str[30];
			Tree*tmp;
			scanf("%s",str);
			tmp=find_word(root,str);
			if(tmp==NULL)
				puts("Not found! \n");
			else
				printf("%s \n",tmp->key->explanation);}
		else if(strcmp(command,"add")==0){
			Word*tmp=(Word*)malloc(sizeof(Word));
			printf("word: ");
			gets(tmp->word);
			printf("class: ");
			gets(tmp->word_class);
			printf("meaning: ");
			gets(tmp->explanation);
			insert_BST(&root,tmp);}
		else if(strcmp(command,"delete")==0){
			char str[30];
			Tree*tmp,*tmp2;
			scanf("%s",str);
			tmp=find_word(root,str);
			if(tmp==NULL)
				printf("Not found!");
			else{
				tmp2=delete_word(&root,tmp);
				printf("%s Deleted successfully. \n",tmp2->key->word);
				free(tmp2);}
		}
		else if(strcmp(command,"deleteall")==0){
			int N=deleteall(&root);
			printf("%d words were deleted successfullly. \n",N);
		}
		else if(strcmp(command,"exit")==0)
			break;
		else
			puts("retry!");
	}
	fclose(fp);
	return 0;
}

void insert_BST(Tree**root,Word*key){
	Tree*x=*root;
	Tree*y=NULL;
	Tree*tmp;
	while(x!=NULL){
		y=x;
		if(strcmp(x->key->word,key->word)>0)
			x=x->left;
		else
			x=x->right;}
	tmp=(Tree*)malloc(sizeof(Tree));
	tmp->key=key;
	tmp->left=NULL;
	tmp->right=NULL;
	tmp->p=y;
	if(y==NULL)
		*root=tmp;
	else if(strcmp(y->key->word,key->word)>0)
		y->left=tmp;
	else
		y->right=tmp;
	count++;
}

Tree* find_word(Tree*x,char str[])
{
	if(x==NULL||strcmp(str,x->key->word)==0) 
		return x;
	if(strcmp(str,x->key->word)<0) 
		return find_word(x->left,str);
	else 
		return find_word(x->right,str);
}

Tree* delete_word(Tree**root,Tree*node){
	Tree*y,*x;
	if(node->left==NULL||node->right==NULL)
		y=node;
	else
		y=successor(node);
	if(y->left!=NULL)
		x=y->left;
	else
		x=y->right;
	if(x!=NULL)
		x->p=y->p;
	if(y->p==NULL)
		*root=x;
	else if(y==y->p->left)
		y->p->left=x;
	else
		y->p->right=x;
	if(y!=node)
		node->key=y->key;
	count--;
	return y;
}

Tree* successor(Tree*node)
{
	Tree*p;
	if(node->right!=NULL)
		return find_min(node->right);
	p=node->p;
	while(node!=NULL&&node==p->right){
		node=p;
		p=node->p;}
	return p;
}

Tree* find_min(Tree*root)
{
	while(root->left!=NULL)
		root=root->left;
	return root;
}

int deleteall(Tree**root)
{
	int N=0;
	char filename[30];
	char deleteWords[10000][30];
	FILE*fp;
	scanf("%s",filename);
	fp=fopen(filename,"rt");
	while(fscanf(fp,"%s",deleteWords[N])!=EOF){
		Tree*tmp=find_word(*root,deleteWords[N]);
		if(tmp!=NULL){
			Tree*tmp2=delete_word(root,tmp);
			free(tmp2);}
		N++;
	}
	fclose(fp);
	return N;
}




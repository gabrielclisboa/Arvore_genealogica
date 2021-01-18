#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

using namespace std;


//Dupla : Gabriel Campos e Matheus Valentin 

//Structs
typedef struct PARENTESCO{//Esta struct serve para manejar os parentescos de cada pessoa, a estrutura de dados utilizada nela foi a lista encadeada ordenada por parentes.(QUESTÃO B)
	
	int ID;  //Id do parente 
	int grau; // Grau do parentesco 
	struct PARENTESCO *prox; 
	
}PARENTESCO;

typedef struct DATA{ //Esta struct serve para armazenar a data de nascimento 
	int dia;
	int mes;
	int ano;
}DATA;

typedef struct PESSOA{ //Esta struct tem a função de armazenar os dados das pessoas que são cadastradas, a estrutura de dados utilizada foi a lista encadeada ordenada,de forma crescente, pela idade.(QUESTÃO A)
	int id;
	int qtdP; //Essa variável serve para quantificar a quantidade de parentes registrados , pois os parentescos e as pessoas são salvas no mesmo arquivo. Então, para que fosse possível ler pessoas separadamente de parentescos será analisado esta variável  
	char nome[30];
	DATA dataP;
	char natura[30];
	int idade;
	struct PESSOA *prox;
    PARENTESCO *parentesco; // A struct parentesco é  colocada dentro da struct pessoa.
	
}PESSOA;

//Funções 
char menu();// Menu com as opções que o usuário pode escolher

void imprimir(PESSOA *cabeca); 		  //Imprime o nome e a idade de todos as pessoas cadastradas em ordem crescente pela idade.(QUESTÃO D)    // 
void tabela(); 				 		 //Imprime a tabela com os "graus" de parentesco dos respectivos tipos de  parentesco          			 // Funções de impressão
void taverna(int grau);      		//Imprime o nome do parentesco do grau fornecido                                              			//
void nome(int id,PESSOA *cabeca);  //Imprime o nome da pessoa do id fornecido                                                              //

PESSOA *cadastro(PESSOA *cabeca); // Função que faz o cadastramento das pessoas.(QUESTÃO A)


PARENTESCO *insereParent(int grau,int idP,PARENTESCO *cabeca); //Função que realiza o cadastro dos parentescos.                                    // Funções relativas ao manejo dos parentescos.(QUESTÃO B)
void pesquisaParentesco(PESSOA *p1,PESSOA *p2, int id1, int id2);//Função que realiza a busca e a impresssão do parentesco entre duas pessoas.     //


void buscaArvore(int id, PESSOA *cabeca); //Realiza a busca e a impressão da árvore genealógica de uma pessoa a partir de seu id.(QUESTÃO C)

void preencheArq(PESSOA *cabeca);																						//Função que escreve os dados das pessoas e do parentesco no arquivo txt		     	 //
PESSOA *carregarArq(PESSOA *cabeca);																				  //Função que carrega os dados das pessoas dos arquivos txt                                  //Funções de manejo de arquivo.(QUESTÃO E)
PESSOA *insereP(PESSOA *cabeca,char *nome,char *naturalidade,int dia,int mes,int ano,int qtdP,int idade,int id);	//Função que insere os dados fornecidos do arquivo txt na lista encadeada                   //

PESSOA *achaPessoa(int id,PESSOA *cabeca);//Retorna a pessoa do id fornecido

int main(){
	PESSOA *cabeca=NULL;// Cabeca
	char op;//Opção do menu
	int id1,id2,grau; 
	PESSOA *aux,*p1,*p2;
	
	cabeca=carregarArq(cabeca);
	do{
		op=menu();
		
		system("cls");
		switch(op){
			case 'C':
				cabeca=cadastro(cabeca);
				break;
			case 'P':
				cout<<"Digite o primeiro id:"<<endl;
				cin>>id1;
				cout<<"Digite o segundo id:"<<endl;
				cin>>id2;
				p1=achaPessoa(id1,cabeca);// Pessoa 1 
				p2=achaPessoa(id2,cabeca);// Pessoa 2
				pesquisaParentesco(p1,p2,id1,id2);
				break;
			case 'B':
				cout<<"Digite o seu id:"<<endl;
				cin>>id1;
				buscaArvore(id1,cabeca);
				break;				
			case 'L':
				imprimir(cabeca);
				break;
			case 'I':
				cout<<"Digite o seu id:"<<endl;
				cin>>id1;
				cout<<"Digite o id de seu parente:"<<endl;
				cin>>id2;
				tabela();//Tabela que informa os parentescos 
				cout << "Digite o grau:"<< endl;
				cin>>grau;
				
				aux=achaPessoa(id1,cabeca);
				aux->parentesco=insereParent(grau,id2,aux->parentesco);
				aux->qtdP++;//Variável que controla a quantidade de parentescos 
				break;	
			case 'S':
				break;
			default: 
				system("cls");
				cout << "Opcao nao existe" << endl;	
				system("pause");
				break;	
		}
		system("cls");	
	}while(op!='S');
	
	preencheArq(cabeca);
	return 0;
}

PESSOA *cadastro(PESSOA *cabeca){
	
	int id,dia,mes,ano,idade,grau,idP,dataAtual;
	char nome[30],natura[30],op;
	
	PESSOA *ant,*p,*novo,*aux;
	novo = (PESSOA *) malloc(sizeof(PESSOA));
	novo->prox=NULL;
	
	//Entrada de dados
	cout << "Digite seu ID:" << endl;
	cin >> id;
		cout << "Digite seu nome:" << endl;
		fflush(stdin);
		gets(nome);
		cout << "Data de nascimento:" << endl;
		cout << "\nDIA:" << endl;
		cin >> dia;
		cout << "\nMES:" << endl;
		cin >> mes;
		cout << "\nANO:" << endl;
		cin >> ano;
		cout << "Digite sua naturalidade:" << endl;
		fflush(stdin);
		gets(natura);
		idade=(ano*360)+(mes*30)+dia;
		dataAtual=(2019*360)+(8*30)+29;
		idade=dataAtual-idade;
		
		novo->id=id;
		novo->dataP.dia=dia;
		novo->dataP.mes=mes;
		novo->dataP.ano=ano;
		strcpy(novo->natura,natura);
		strcpy(novo->nome,nome);
		novo->idade=idade;
		novo->parentesco=NULL;
		novo->qtdP=0;
		
		system("cls");
		
		aux=cabeca;
		
		//Inserindo a nova pessoa na lista encadeada de acordo com a idade 
		if(cabeca == NULL)
			cabeca = novo;
		else{
			ant = NULL;
			p = cabeca;
			while((p!=NULL) && (p->idade < idade)){
				ant = p;
				p = p->prox;
			}
			if(ant == NULL)
				cabeca = novo;
			else
				ant->prox = novo;
				
			novo->prox = p;
		}
	return cabeca;
}

PESSOA *achaPessoa(int id,PESSOA *cabeca){
	PESSOA *aux;			
	aux=cabeca;
						
	while(aux->id!=id){ //Até encontrar o id desejado 
		aux=aux->prox;
	}
	return aux;
}

																											//FUNÇÕES DE MANEJAMENTO DE PARENTESCO
																											
void pesquisaParentesco(PESSOA *p1,PESSOA *p2, int id1, int id2){
	
	PARENTESCO *aux=p1->parentesco;
	
			while(aux!=NULL){ //Enquanto o parentesco não for nulo
				if(aux->ID==id2){
					cout<<"O parentesco de ";
					nome(id1,p1);
					cout<<" com ";
					nome(id2,p2);
					cout<<" eh de ";
					taverna(aux->grau);
					cout<<endl;
					system("pause");
					return;	
				}	
				aux=aux->prox;
			}
			aux=p2->parentesco;
			while(aux!=NULL){ //Enquanto o parentesco não for nulo
				if(aux->ID==id1){
					cout<<"O parentesco de ";
					nome(id2,p2);
					cout<<" com ";
					nome(id1,p1);
					cout<<" eh de ";
					taverna(aux->grau);
					cout<<endl;
					system("pause");
					return;	
				}	
				aux=aux->prox;
			}
	cout<<"Nao ha parentesco!"<<endl;
	system("pause");
}

void buscaArvore(int id, PESSOA *cabeca){
	PESSOA *aux;
	aux=achaPessoa(id,cabeca); 	
	PARENTESCO *aux2;
	aux2=aux->parentesco;
	
	while(aux2!=NULL){// Enquanto o parentesco não for nulo
		taverna(aux2->grau);
		nome(aux2->ID,cabeca);
		printf("\n");
		aux2=aux2->prox;
	}
	printf("\n");
	system("pause");
}

PARENTESCO *insereParent(int grau,int idP,PARENTESCO *cabeca){
	
	PARENTESCO *novo=(PARENTESCO*)malloc (sizeof(PARENTESCO));
	PARENTESCO	*ant,*p,*aux;
	aux=cabeca;
	
	novo->grau=grau;
	novo->ID=idP;
	novo->prox=NULL;
	
	if(cabeca == NULL)
		cabeca = novo;
	else{
		ant = NULL;
		p = cabeca;
		while((p!=NULL) && (p->grau < grau)){
			ant = p;
			p = p->prox;
		}
		if(ant == NULL)
			cabeca = novo;
		else
			ant->prox = novo;
			
		novo->prox = p;
		}
	return cabeca;
}

																										//FUNÇÕES DE MANEJAMENTO DE ARQUIVO
																										
PESSOA *insereP(PESSOA *cabeca,char *nome,char *naturalidade,int dia,int mes,int ano,int qtdP,int idade,int id){ 
	PESSOA *novo,*aux;
	novo = (PESSOA *) malloc(sizeof(PESSOA));
	
	aux=cabeca;
	
	novo->id=id;
	novo->dataP.dia=dia;
	novo->dataP.mes=mes;
	novo->dataP.ano=ano;
	strcpy(novo->natura,naturalidade);
	strcpy(novo->nome,nome);
	novo->idade=idade;
	novo->parentesco=NULL;
	novo->qtdP=qtdP;
	novo->prox=NULL;
	
	// É interessante ressaltar que as pessoas são inseridas como se fosse uma lista encadeada normal , pois elas foram inseridas no arquivo já ordenadas.
	if(cabeca==NULL){
		cabeca=novo;
		return cabeca;
	}else{
		while(aux->prox!=NULL){
			aux=aux->prox;
		}
		aux->prox=novo;
	}
	return cabeca;
}

PESSOA *carregarArq(PESSOA *cabeca){ 
	
	int idP,grau,i=0,mes,ano,dia,id,idade,qtdP;
	char buf[110],*nome,*naturalidade;
	FILE *arq=fopen("dados.txt","r"); 
	PESSOA *aux;
	
	fgets(buf,110,arq);                 
	while(!feof(arq)){	// enquanto o arquivo não acabar 
	
		id=atoi(strtok(buf,";"));			 
		nome=strtok(NULL,";"); 
		dia=atoi(strtok(NULL,";"));	
		mes=atoi(strtok(NULL,";"));	
		ano=atoi(strtok(NULL,";"));
		naturalidade=strtok(NULL,";");
		idade=atoi(strtok(NULL,";"));
		qtdP=atoi(strtok(NULL,";"));//Variável que determina a quantidade de parentescos que a pessoa tem 

		cabeca=insereP(cabeca,nome,naturalidade,dia,mes,ano,qtdP,idade,id); // Enviando os dados obtidos do arquivo txt e a cabeca para a função que vai realizar a inserção de um novo elemento na lista encadeada cabeca
		
		while(i<qtdP){  //É analisado se a pessoa , registrada na cabeca , possui algum parentesco. Caso haja , ocorrerá a leitura dos dados e o registro deste parentesco na lista encadeada da pessoa registrada.
			fgets(buf,110,arq);
			idP=atoi(strtok(buf,";"));
			grau=atoi(strtok(NULL,";"));
			aux=achaPessoa(id,cabeca);
			aux->parentesco=insereParent(grau,idP,aux->parentesco);	
			i++;
		}
		i=0;// Variável de controle que ajuda no registo dos parentescos 		
						
		fgets(buf,110,arq);//"Pulou" a linha 
	}
	fclose(arq);
	return cabeca;	
}

void preencheArq(PESSOA *cabeca){
	int idade,id,idP,grau,qtdP,ano,dia,mes;
	char nome[80],naturalidade[80];
	
	FILE *arq=fopen("dados.txt","w");
	 
	while(cabeca!=NULL){      // Percorre a lista encadeada de pessoas 
		ano=cabeca->dataP.ano;
		mes=cabeca->dataP.mes;
		dia=cabeca->dataP.dia;
		strcpy(nome,cabeca->nome);
		id=cabeca->id;
		strcpy(naturalidade,cabeca->natura);
		qtdP=cabeca->qtdP;
		idade=cabeca->idade;
		fprintf(arq,"%d;%s;%d;%d;%d;%s;%d;%d\n",id,nome,dia,mes,ano,naturalidade,idade,qtdP);//Escrevendo os arquivos no arquivo txt
	
		while(cabeca->parentesco!=NULL){ //Percorre a lista encandeada de parentesco
			idP=cabeca->parentesco->ID;
			grau=cabeca->parentesco->grau;
			fprintf(arq,"%d;%d\n",idP,grau); // Escrevendo os parentescos no mesmo arquivo txt 
			cabeca->parentesco=cabeca->parentesco->prox;
		}
		cabeca=cabeca->prox;
	}
	fclose(arq);
}

																												//FUNÇÕES DE IMPRESSÃO
																												
char menu(){
	char op;
	
	cout << "(C)adastrar Pessoa" << endl;
	cout << "(P)esquisar Parentesco" << endl;
	cout << "(B)uscar Arvore genealogica" << endl;
	cout << "(L)istar Pessoas" << endl;
	cout << "(I)nserir Parentesco"<< endl;
	cout << "(S)air" << endl;
	cout << ">";
	
	fflush(stdin);
	cin >>op;
	
	return toupper(op);
}

void imprimir(PESSOA *cabeca){
	PESSOA *aux=cabeca;
	
	while(aux!=NULL){
		cout << "NOME:"<<aux->nome<<" IDADE:"<<(aux->idade/360) << endl;
		aux=aux->prox;
	}
	system("pause");
	
}

void nome(int id,PESSOA *cabeca){
	PESSOA *aux;
	aux=cabeca;
	while(aux!=NULL){
		if(aux->id==id){
			cout<<aux->nome;
			return;
		}
		aux=aux->prox;
	}
}

void taverna(int grau){
	switch(grau){
		case 1:
			cout<<"Avo ";
		break;	
		case 2:
			cout<<"AvO ";
		break;
		case 3:
			cout<<"Tio ";
		break;
		case 4:
			cout<<"Tia ";
		break;
		case 5:
			cout<<"Pai ";
		break;
		case 6:
			cout<<"Mae ";
		break;
		case 7:
			cout<<"Filho ";
		break;
		case 8:
			cout<<"Filha ";
		break;	
		case 9:
			cout<<"Primo ";
		break;
		case 10:
			cout<<"Prima ";
		break;
		case 11:
			cout<<"Sobrinho ";
		break;
		case 12:
			cout<<"Sobrinha ";
		break;
		case 13:
			cout<<"Irmao ";
		break;
		case 14:
			cout<<"Irma ";
		break;
		case 15:
			cout<<"Neto ";
		break;
		case 19:
			cout<<"Neta ";
		break;														
	}
}

void tabela(){
	cout << "\t\tTABELA DE PARENTESCO" << endl;
	cout << "|1|   Avo" <<endl;
	cout << "|2|   AvO" <<endl;
	cout << "|3|   Tio" <<endl;
	cout << "|4|   Tia" <<endl;
	cout << "|5|   Pai" <<endl;
	cout << "|6|   Mae" <<endl;
	cout << "|7|   Filho" <<endl;
	cout << "|8|   Filha" <<endl;
	cout << "|9|   Primo" <<endl;
	cout << "|10|  Prima" <<endl;
	cout << "|11|  Sobrinho" <<endl;
	cout << "|12|  Sobrinha" <<endl;
	cout << "|13|  Irmao" <<endl;
	cout << "|14|  Irma" <<endl;
	cout << "|15|  Neto" <<endl;
	cout << "|16|  Neta" <<endl;	
}

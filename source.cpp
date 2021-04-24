#include <iostream>
using namespace std;

char comando[5] = {'c','l','e','a','r'};

struct Carta{
  int numero;
  string naipe;
  string cor;
};
struct No{
  Carta carta;
  No *prox;
  No *ant;
};


typedef No* noPtr;

Carta cartas[52];
noPtr topo[8];
noPtr localTemporario = NULL;
Carta localDefinitivo[4] = {{0,"♡"}, { 0 ,"♢"}, {0, "♠"}, {0, "♣"}}; //talvez funcione
noPtr ultimo[8];

bool fimDeJogo();
bool logicaMoverNoDoTempParaDefinitivo(Carta origem);
bool logicaMoverNo(Carta origem, Carta destino);
void embaralha();
void inserir(int i, Carta carta);
void tela();
void inserirNo(noPtr topo);
void inicializa();
void moverNo(int pilhaAntiga, int novaPilha);
void geraCartas();
void moverNoParaTemp(int pilhaAntiga);
void moverDoTempParaPilha();
void moverParaLocalDefinitivo(int pilhaEscolhida);
void moverDoTempParaDefinitivo();

int main() {
  inicializa();//lista encadeada
  geraCartas();
  embaralha();

  int pilha = 0;
  for(int i=0 ; i <52;i++){ //Coloca as cartas na lista encadeada;
    inserir(pilha,cartas[i]);
    pilha++;
    if(pilha==8)
      pilha = 0;
  }

  int opcao=0;
  int pilhaAntiga,  novaPilha;

  int config;

  cout<<"Sistema operacional que esta rodando:\n1-WINDOWS\n2-LINUX\n";
  cin>>config;

  if(config==1){
    comando[2] = 's';
    comando[3] = '\0';
  }


  do{

    tela();
    cout<<"1 -Mover uma carta de uma pilha para outra\n";
    cout<<"2 - Mover para local temporario\n";
    cout<<"3 - Mover do temporario para a pilha\n";
    cout<<"4 - Mover para local definitivo\n";
    cout<<"5 - Mover do temporario para espaco definitivo\n";
    cout<<"Selecione uma opcao: ";
    cin>>opcao;
    switch(opcao){
      case 1: 
      cout<<"Selecione uma pilha pra mover o topo: ";
      cin>>pilhaAntiga;
      cout<<"Selecione uma pilha para colocar a carta selecionada: ";
      cin>>novaPilha;
      system(comando);
      moverNo(pilhaAntiga, novaPilha); 
      break;
      case 2:
      cout<<"De qual Pilha deseja tirar?  ";
      cin>>pilhaAntiga;
      system(comando);
      moverNoParaTemp(pilhaAntiga);
      break;
      case 3:
      moverDoTempParaPilha();
      break;
      case 4:
      cout<<"De qual Pilha deseja tirar?  ";
      cin>>pilhaAntiga;
      moverParaLocalDefinitivo(pilhaAntiga);
      break;
      case 5:
      moverDoTempParaDefinitivo();
      break;
    }
  }while(opcao!=0 && !fimDeJogo());

}
//
void tela(){

  noPtr *aux = ultimo, aux2; //Ultimo eh um vetor[8] que possui 8 listas encadeadas
  cout<<"*********** PILHAS ************\n";
  
  for(int i = 0; i<13; i++){ // Numero de cartas em cada pilha
      for(int j=0 ; j<8; j++){ // Iterando no vetor de pilhas
        int k = 0;
        noPtr auxNo = ultimo[j]; // para nao mudar a avariavel
        while(k<i && auxNo!=NULL){
          if(auxNo!=NULL){
            auxNo = auxNo->ant;//encontra o elemento
            k++;
          }
        }
        if(k==i && auxNo){
          switch(auxNo->carta.numero){
            case 1: cout<<"A" ; break;
            case 11: cout<<"J"; break;
            case 12: cout<<"Q"; break;
            case 13: cout<<"K"; break;
            default: cout<<auxNo->carta.numero;
          }
          
          cout<<auxNo->carta.naipe; 
          if(auxNo->carta.numero!=10)
            cout<< "  | "; 
          else
            cout<< " | ";
        } else{
          cout<<"    | ";
      } 
    }
    if(i==1)
      cout<<"      Maximo de 8 posicoes !";
    else if(i==2){
      cout<<" -------  ESPACO TEMPORARIO  ------- ";
    }
    else if(i==3){
      noPtr tempAux = localTemporario;
      cout<< "    ";
      while(tempAux){
        cout<<" ";

        switch(tempAux->carta.numero){
            case 1: cout<<"A" ; break;
            case 11: cout<<"Q"; break;
            case 12: cout<<"J"; break;
            case 13: cout<<"K"; break;
            default: cout<<tempAux->carta.numero;
          }
        cout<<tempAux->carta.naipe<<"|";
        tempAux= tempAux->prox;
      }
      
    } else if(i == 5){
      cout<<" ---  ESPACO DEFINITIVO ---";
    } else if(i==6){
      cout<< "       ";
      for(int i=0; i<4 ; i++){
            
        if(!localDefinitivo[i].numero){
          switch(i){
                case 0: cout<<"♡" ; break;
                case 1: cout<<"♢"; break;
                case 2: cout<<"♠"; break;
                case 3: cout<<"♣"; break;
          }
        } else{
          switch(localDefinitivo[i].numero){
            case 1: cout<<"A" ; break;
            case 11: cout<<"Q"; break;
            case 12: cout<<"J"; break;
            case 13: cout<<"K"; break;
            default: cout<<localDefinitivo[i].numero;
          }
          cout<<localDefinitivo[i].naipe;
        }
        cout<<" | ";
      }    
    }

    cout<<endl;
  }
  cout<<endl;
  cout<<" 1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |\n";
}
void inicializa(){
  for(int i=0 ; i<8; i++){
    topo[i] = NULL;
  }
  
}
void moverNo(int pilhaAntiga, int novaPilha){
  
  if(pilhaAntiga >= 1 && pilhaAntiga <=8 && novaPilha >= 1 && novaPilha<=8){
  pilhaAntiga -= 1;
  novaPilha -= 1;
  
  bool teste;
    if(topo[pilhaAntiga] !=NULL ){
     // cout<<pilhaAntiga<<novaPilha;
        if(topo[novaPilha]){
        teste = logicaMoverNo(topo[pilhaAntiga]->carta, topo[novaPilha]->carta);
        //da segmentation fault se a pilha de destino esta vazia.
        }else {
          teste = true; //Caso a pilha de destino esteja vazia
        } 
        if(teste){ 
        noPtr aux = topo[pilhaAntiga], aux2; 
        topo[pilhaAntiga] = topo[pilhaAntiga]->prox; 
          
          if(topo[pilhaAntiga] != NULL) ////TopoAtualAgr= ant recebe NUll
            topo[pilhaAntiga]->ant = NULL;
          else 
            ultimo[pilhaAntiga] = NULL;

          if(topo[novaPilha] != NULL){
          aux2 = topo[novaPilha];
          topo[novaPilha]->ant = aux ;
          topo[novaPilha] = aux;
          topo[novaPilha]->prox = aux2;
          } else{
            topo[novaPilha] = aux;
            topo[novaPilha]->ant =NULL;
            topo[novaPilha]->prox = NULL;
            ultimo[novaPilha] = topo[novaPilha];
          }
       } 
    } else{
      cout<<"Pilha selecionada esta vazia.\n";
      }

  } else{
    cout<<"Escolha de 1 ate 8!\n";
  }
}
//cout<<"2♠,♡,♢,♣";

void inserir(int i,Carta carta){
  noPtr novoNo = new No;
  novoNo->prox=NULL;
  novoNo->ant = NULL;  
  
  novoNo->carta = carta;
  
  if(topo[i] == NULL){
    topo[i] =novoNo;
    ultimo[i] = novoNo;
  } else{
    topo[i]->ant = novoNo;
    novoNo->prox = topo[i];
    topo[i] = novoNo;
  }
}

void geraCartas(){
 
  int j= 0;
  int k= 1;
    for(int i=0; i<52; i++){
        j++;
        cartas[i].numero = j;
        switch(k){
        case 1:cartas[i].naipe = "♠"; cartas[i].cor = "black"; break;
        case 2:cartas[i].naipe = "♣"; cartas[i].cor = "black";break;
        case 3:cartas[i].naipe = "♡"; cartas[i].cor = "red";break;
        case 4:cartas[i].naipe = "♢"; cartas[i].cor = "red";break;  
        } 
     
        if(j%13 == 0 && i != 1){
          j=0;  // para as cartas numero
          k++;//para o switch
        }
             
      }    
}
void embaralha(){
  srand(time(0));
	for (int i = 0; i <52; i++)
	{
    Carta *temp = new Carta;
		int r =  (rand() % 52);
    //cout<<r<<" ";
    temp->numero = cartas[i].numero;
    temp->naipe = cartas[i].naipe;
    temp->cor = cartas[i].cor;
    cartas[i].numero = cartas[r].numero;
    cartas[i].naipe = cartas[r].naipe;
    cartas[i].cor = cartas[r].cor;
    cartas[r].numero = temp->numero;
    cartas[r].naipe = temp->naipe;
    cartas[r].cor = temp->cor;
    
	}
}
void moverNoParaTemp(int pilhaAntiga){
    

    if(pilhaAntiga >= 1 && pilhaAntiga <=8 && topo[pilhaAntiga-1] != NULL){
      pilhaAntiga -= 1;
     // noPtr ant = topo[pilhaAntiga];
      if(topo[pilhaAntiga] != NULL){
          if(localTemporario == NULL){
            localTemporario = topo[pilhaAntiga];
            topo[pilhaAntiga] = topo[pilhaAntiga]->prox;
            
            if(topo[pilhaAntiga]){
            topo[pilhaAntiga]->ant = NULL;
            localTemporario->prox = NULL;
            localTemporario->ant = NULL;
            } 
          } 
          else{
            noPtr aux = localTemporario, ant ;
            int i=1;
            while(aux->prox != NULL && i < 8){
              aux = aux->prox;
              i++;
            }
            if(i==8){
              cout<<"Espaco temporario cheio.\n";
            } else{
              aux->prox = topo[pilhaAntiga];
              topo[pilhaAntiga] = topo[pilhaAntiga]->prox;
              if(topo[pilhaAntiga] == NULL){
                ultimo[pilhaAntiga] = NULL;
              } else{
              topo[pilhaAntiga]->ant = NULL;
              }
              ant = aux;
              aux = aux->prox;
              aux->ant = ant;
              aux->prox = NULL;
            }
          } 
      } else{
        cout<<"Pilha escolhida vazia.\n";
      }
    }
    if(topo[pilhaAntiga] == NULL){
      ultimo[pilhaAntiga] = NULL;
    }
}

void moverDoTempParaPilha(){
  int novaPilha, posicao, i =0; 
  noPtr aux=  localTemporario;
  bool teste = true;

  cout<<"Escolha uma posicao de 1 a 8 do espaco Temporario: ";
  cin>>posicao;
  cout<<"Deseja em qual pilha: ";
  cin>>novaPilha;
  system(comando);
  
  if(posicao>=1 && posicao<=8 && novaPilha>=1 && novaPilha<=8 &&aux){
    bool mover = false;
    for( i=1; i<posicao && aux->prox; i++){
      
      aux = aux->prox;
    }
    if(!topo[novaPilha -1 ] && aux){ // caso a pilha de destino nao tenha elementos 
      topo[novaPilha-1] = aux;
      ultimo[novaPilha-1] = aux;
      mover = true;
    } else if (posicao == i && aux){
      mover = logicaMoverNo(aux->carta, topo[novaPilha-1]->carta);     
    } 
    
    if(mover && aux){
      if(aux == localTemporario){
        if(aux->prox){
          localTemporario= localTemporario->prox;
          localTemporario->ant = NULL;
        } else{
          localTemporario = NULL;
        }         
      } 
      else if(!aux->prox){
        aux->ant->prox = NULL;
      } 
      else{
        aux->ant->prox = aux->prox;
        aux->prox->ant = aux->ant;
      }

      topo[novaPilha-1]->ant = aux;
      aux->prox = topo[novaPilha-1];
      aux->ant = NULL;
      topo[novaPilha-1] = aux;
 
    } 
    else{
      cout<<"Posicao escolhida invalida ou movimento invalido.\n";
    }
  }

}
     
 

void  moverParaLocalDefinitivo(int pilhaEscolhida){
  string naipes[4] = {"♡", "♢", "♠" , "♣"};
  bool flag = true; 
  
 
  if(pilhaEscolhida >= 1 && pilhaEscolhida <=8 && topo[pilhaEscolhida-1] != NULL){
    pilhaEscolhida--;
    for(int i=0; i<4 && flag; i++){ 
      
      if  (topo[pilhaEscolhida] && topo[pilhaEscolhida]->carta.naipe == naipes[i] ){
        flag = false;
          system(comando);  
          if(localDefinitivo[i].numero + 1 == topo[pilhaEscolhida]->carta.numero) {
          // testo se o numero da carta anterior + 1 é igual o numero da carta atual;
            localDefinitivo[i] = topo[pilhaEscolhida]->carta;
            
            //tiro da pilha e deleto
            noPtr aux = topo[pilhaEscolhida] ;

            topo[pilhaEscolhida] = topo[pilhaEscolhida]->prox;
            if(topo[pilhaEscolhida]){// Caso tenha ainda elemento // topo tem anterior == NULL
                topo[pilhaEscolhida]->ant = NULL;
            }
            delete aux;
            system(comando);
          } else {
            system("clear");
            cout<<"Movimento invalido\n";
          }

        }
    }
  } else{
    system(comando);
    cout<<"Escolha de 1 ate 8";
  }

  if(flag){
    cout<<"Movimento invalido.\n";
  }

}


bool logicaMoverNo(Carta origem, Carta destino){

  
    if(origem.cor != destino.cor && origem.numero + 1 == destino.numero)
      return true;
    return false;
}

void moverDoTempParaDefinitivo(){
  string naipes[4] = {"♡", "♢", "♠" , "♣"};
  int posicao; 
  noPtr aux=  localTemporario, cartaSalva= NULL;
  cout<<"Escolha uma posicao de 1 a 8 do espaco Temporario: ";
  cin>>posicao;
  system(comando);
 
  for(int i=1; i<posicao &&aux;i++){
      aux = aux->prox;
  }
  if(!aux){
    cout<<"Posicao escolhida invalida ou espaco temporario vazio\n";
  } else{
    //primeiro testo se posso inserir a carta no local definitivo

    bool mover = logicaMoverNoDoTempParaDefinitivo(aux->carta);
    if(mover){
      if(aux == localTemporario){
        
        if(aux->prox){
          localTemporario= localTemporario->prox;
          localTemporario->ant = NULL;
        } else{
          localTemporario = NULL;
        }         
      } else if(!aux->prox){
        aux->ant->prox = NULL;
      } else{
        aux->ant->prox = aux->prox;
        aux->prox->ant = aux->ant;
      }
      delete aux;


    } else{
      cout<<"Movimento invalido.\n";
    }
  }
} 
  
bool logicaMoverNoDoTempParaDefinitivo(Carta origem){
    
    for(int i =0; i<4; i++){
      if(origem.naipe == localDefinitivo[i].naipe )
        if(localDefinitivo[i].numero + 1 == origem.numero){
          localDefinitivo[i] = origem;
          return true;
        }
    }
    return false;
}
bool fimDeJogo(){
    for(int i=0 ; i<4 ; i++){
      if(localDefinitivo[i].numero != 13)
        return false;
    }

    cout<<" ******  '\' O / VOCE GANHOU '\' O / ******" ;
    return true;
}

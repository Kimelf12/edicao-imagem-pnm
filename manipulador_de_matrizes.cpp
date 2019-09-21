/*
INF110 - Trabalho pratico 3
programa para ler, modificar e gravar uma imagem no formato PNM

Autores: Andre Gustavo dos Santos         (criado em 16/06/14)
         Andre Gustavo dos Santos         (modificado em 22/05/18)
         Andre Gustavo dos Santos         (modificado em 21/05/19)
         Erick Lima Figueiredo			  (modificado em 28/05/19)
*/

//Bibliotecas
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
//*****************//

using namespace std;

//Valores maximos de altura e largura
const int MAXALTURA  = 700;
const int MAXLARGURA = 700;

//Apesar de nao ser uma boa pratica precisava de mais memoria
int aux[MAXALTURA][MAXLARGURA] = {255};			//auxiliar para a criacao de sobel preto e branco
int auxc[MAXALTURA][MAXLARGURA][3] = {0};		//auxiliar para a criacao de sobel colorido

//*** PROTOTIPO DAS FUNCOES ***//

//Cinza
void batman_img(unsigned char img[][700], int aux[][700], int alt, int larg);
void clarear_img(unsigned char img[][700], int alt, int larg);
void convergir_img(unsigned char img[][700], int alt, int larg);
void divergir_img(unsigned char img[][700], int alt, int larg);
void escurecer_img(unsigned char img[][700], int alt, int larg);
void espelhar_img(unsigned char img[][700], int alt, int larg);
void laplace_img(unsigned char img[][700], int aux[][700], int alt, int larg);
void mescla_img(unsigned char img[][700], int alt, int larg);
void negativo_img(unsigned char img[][700], int alt, int larg);
void pretoebranco_img(unsigned char img[][700], int alt, int larg);
void sobel_img(unsigned char img[][700], int aux[][700], int alt, int larg);
//***************************************************************//

//Colorido
void clarear_img(unsigned char img[][700][3], int alt, int larg);
void convergir_img(unsigned char img[][700][3], int alt, int larg);
void divergir_img(unsigned char img[][700][3], int alt, int larg);
void escurecer_img(unsigned char img[][700][3], int alt, int larg);
void espelhar_img(unsigned char img[][700][3], int alt, int larg);
void laplace_img(unsigned char img[][700][3], int aux[][700][3], int alt, int larg);
void mescla_img(unsigned char img[][700][3], int alt, int larg);
void negativo_img(unsigned char img[][700][3], int alt, int larg);
void sobel_img(unsigned char img[][700][3], int aux[][700][3], int alt, int larg);
void tonsdecinza_img(unsigned char img[][700][3], unsigned char nova_img[][700], int alt, int larg, string & tipo);
void batman_img(unsigned char img[][700][3], int aux[][700], int alt, int larg);
//***************************************************************//

int main() {
	//Declaracao das variaveis
	unsigned char imagem[MAXALTURA][MAXLARGURA];	//imagem em preto e branco
	unsigned char imagemc[MAXALTURA][MAXLARGURA][3];//imagem colorida
	int largura, altura;							//dimensoes da imagem
	ifstream arqentrada;							//arquivo que contem a imagem original
	ofstream arqsaida;								//arquivo que contera a imagem gerada
	char comentario[200], c;						//auxiliares
	int i, j, k, valor, op;							//auxiliares
	string local, tipo, arqsalve;					//rotulacoes de arquivos
	bool grava;										//flag de gravacao
	//***************************************************************//

	//*** LEITURA DA IMAGEM ***//
	//*** Abertura do arquivo ***//
	do {
		grava = true;								// Informa que o arquivo pode ser gravado
		do {
			cout << "MANIPULACAO DE IMAGEM\n\nInforme apenas o nome do arquivo (este precisa estar dentro da mesma pasta do .exe): ";
			cin >> local;
			local += ".pnm";
			arqentrada.open(local.c_str(), ios::in); //Abre arquivo para leitura
			if (!arqentrada) {
				cout << "Erro ao abrir o arquivo '" << local << "'.\n\n:^(\n\n\n";
			}
		} while(!arqentrada);						//Repete o laco até que um arquivo consiga ser aberto
		system("cls||clear");						//Limpar o prompt de comando

		//*** Leitura do cabecalho ***//
		arqentrada >> tipo;							//Le o tipo de arquivo
		arqentrada.get();							//Le e descarta o \n do final da 1a. linha

		if (tipo == "P2") {							
			cout << "\nDetectado: Imagem em tons de cinza\n";
		} else if (tipo == "P3") {
			cout << "\nDetectado: Imagem colorida\n";
		} else if ((tipo == "P5") || (tipo == "P6")) {
			cout << "Detectado: Imagem no formato RAW\n";
			cout << "Erro: Formato de arquivo nao suportado.\n";
			arqentrada.close();
			return 0;
		}

		while((c = arqentrada.get()) == '#')							//Enquanto for comentario
			arqentrada.getline(comentario, 200);						//Le e descarta a linha "inteira"

		arqentrada.putback(c);											//Devolve o caractere lido para a entrada, pois como
		//nao era comentario, era o primeiro digito da largura

		arqentrada >> largura >> altura;								//Le o numero de pixels da horizontal e vertical
		cout << "Tamanho: " << largura << " x " << altura << endl;
		if (largura > MAXLARGURA) {
			cout << "Erro: Imagens com mais de " << MAXLARGURA << " pixels de largura nao sao suportadas.\n";
			arqentrada.close();
			return 0;
		}
		if (largura > MAXALTURA) {
			cout << "Erro: Imagens com mais de " << MAXLARGURA << " pixels de altura nao sao suportadas.\n";
			arqentrada.close();
			return 0;
		}

		arqentrada >> valor;											//Valor maximo do pixel (temos que ler, mas nao sera usado)
		//****************************//


//*** FIM DA LEITURA DA IMAGEM ***//

		if(tipo == "P3") {
			//*** Leitura dos pixels da imagem ***//
			for(i = 0; i < altura; i++) {
				for(j = 0; j < largura; j++) {
					for(k = 0; k < 3; k++) {
						arqentrada >> valor;
						imagemc[i][j][k] = (unsigned char)valor;
					}
				}
			}

			//************************************//

			arqentrada.close();

			do {
				cout << "\nMENU DE SELECAO\nArquivo: " << local << "\n\n\n 1- Batman\n 2- Clarear\n 3- Convergir\n 4- Divergir\n 5- Escurecer\n 6- Espelhar Verticalmente\n 7- Laplace\n 8- Mesclagem\n 9- Negativo\n 10- Preto e Branco\n 11- Sobel\n 12- Tons de Cinza\n 13- Sair\n\n Opcao: ";
				cin >> op;												//Informa qual manipulacao sera feita
				cout << endl;
			} while((op < 1) || (op > 13));								//Repete o laco ate um valor no intervalo seja inserido
			switch(op) {
			case 1:
				if((largura > 300) && (altura > 250)) {					//Verifica se a imagem tem os requisitos minimos para a aplicacao do filtro
					batman_img(imagemc, aux, altura, largura);
				} else {
					grava = false;										//Flag de gravacao recebe falso
				}
				break;
			case 2:
				clarear_img(imagemc, altura, largura);
				break;
			case 3:
				convergir_img(imagemc, altura, largura);
				break;
			case 4:
				divergir_img(imagemc, altura, largura);
				break;
			case 5:
				escurecer_img(imagemc, altura, largura);
				break;
			case 6:
				espelhar_img(imagemc, altura, largura);
				break;
			case 7:
				laplace_img(imagemc, auxc, altura, largura);
				break;
			case 8:
				mescla_img(imagemc, altura, largura);
				break;
			case 9:
				negativo_img(imagemc, altura, largura);
				break;
			case 10:
				tonsdecinza_img(imagemc, imagem, altura, largura, tipo);		//Passa a imagem colorida para tons de cinza
				pretoebranco_img(imagem, altura, largura);						//Passa essa nova imagem para os extremos preto ou branco
				break;
			case 11:
				sobel_img(imagemc, auxc, altura, largura);
				break;
			case 12:
				tonsdecinza_img(imagemc, imagem, altura, largura, tipo);
				break;
			case 13:
				cout << "Encerrando...";
				return 0;
				break;
			}
		} else {
			//*** Leitura dos pixels da imagem ***//
			for(i = 0; i < altura; i++) {
				for(j = 0; j < largura; j++) {
					arqentrada >> valor;
					imagem[i][j] = (unsigned char)valor;
				}
			}

			//************************************//
			
			arqentrada.close();

			do {
				cout << "\nMENU DE SELECAO\nArquivo: " << local << "\n\n\ 1- Batman\n 2- Clarear\n 3- Convergir\n 4- Divergir\n 5- Escurecer\n 6- Espelhar Verticalmente\n 7- Laplace\n 8- Mesclagem\n 9- Negativo\n 10- Preto e Branco\n 11- Sobel\n 12- Sair\n\n Opcao: ";
				cin >> op;
				cout << endl;
			} while((op < 1) || (op > 12));
			switch(op) {
			case 1:
				if((largura > 300) && (altura > 250)) {						//Verifica se a imagem tem os requisitos minimos para a aplicacao do filtro
					batman_img(imagem, aux, altura, largura);
				} else {
					grava = false;											//Flag de gravacao recebe falso
				}
				break;
			case 2:
				clarear_img(imagem, altura, largura);
				break;
			case 3:
				convergir_img(imagem, altura, largura);
				break;
			case 4:
				divergir_img(imagem, altura, largura);
				break;
			case 5:
				escurecer_img(imagem, altura, largura);
				break;
			case 6:
				espelhar_img(imagem, altura, largura);
				break;
			case 7:
				laplace_img(imagem, aux, altura, largura);
				break;
			case 8:
				mescla_img(imagem, altura, largura);
				break;
			case 9:
				negativo_img(imagem, altura, largura);
				break;
			case 10:
				pretoebranco_img(imagem, altura, largura);				//Passa essa nova imagem para os extremos preto ou branco
				break;
			case 11:
				sobel_img(imagem, aux, altura, largura);
				break;
			case 12:
				cout << "Encerrando...";
				return 0;
				break;
			}
		}

//*** FIM DO TRATAMENTO DA IMAGEM ***//
//*** GRAVACAO DA IMAGEM ***//
		//inicialmente nao sera necessario entender nem mudar nada nesta parte

		//*** Grava a nova imagem ***//
		if(grava) {										//Se a flag estiver true, a imagem pode ser gravada
			cout << "Informe apenas o nome do arquivo a ser gravado: ";
			cin >> arqsalve;							//Recebe o nome do arquivo a ser gravado
			arqsalve += ".pnm";							//Adiciona pnm ao nome da imagem
			arqsaida.open(arqsalve.c_str(), ios::out);	//Abre arquivo para escrita
			if (!arqsaida) {
				cout << "Nao consegui criar " << arqsalve << ".";
			} else {
				arqsaida << tipo << endl;						//tipo

				arqsaida << "# INF110 - Criado por André Gustavo e Erick Lima Figueiredo - 2019\n";						//comentario
				arqsaida << largura << " " << altura << endl;	//dimensoes
				arqsaida << 255 << endl;						//maior valor
				if(tipo == "P3") {
					for(i = 0; i < altura; i++)
						for(j = 0; j < largura; j++)
							for(k = 0; k < 3; k++)
								arqsaida << (int)imagemc[i][j][k] << endl;	//pixels
				} else {
					for(i = 0; i < altura; i++)
						for(j = 0; j < largura; j++)
							arqsaida << (int)imagem[i][j] << endl;	//pixels
				}
				arqsaida.close();		//fecha o arquivo
				do {
					system("cls||clear");
					cout << arqsalve << " foi criado com sucesso!\n\nDeseja realizar outra manipulacao? (1- Sim | 2- Nao): ";
					cin >> op;
					if(op == 1) {
						break;
					}
					if(op == 2) {
						return 0;
					}
				} while((op < 1) || (op > 2));
				system("cls||clear");
			}
			//***************************//
		} else {											//Se a flag estiver false, a imagem nao pode ser gravada
			do {
				system("cls||clear");
				cout << "Deseja realizar outra manipulacao? (1- Sim | 2- Nao): ";
				cin >> op;									//Le se o usuario 
				if(op == 1) {
					break;
				}
				if(op == 2) {
					return 0;
				}
			} while((op < 1) || (op > 2));
			system("cls||clear");
		}
//*** FIM DA GRAVACAO DA IMAGEM ***//
	} while(true);
	return 0;
}

//*** Funcoes ***//

//Cinza

void batman_img(unsigned char img[][700], int aux[][700], int alt, int larg) {
	ifstream arqentrada;						//Auxiliares
	int valor, distx, disty, v;					//Auxiliares
	arqentrada.open("batman.pnm", ios::in);		//Abre os valores da matriz de duas dimensões contidas no arquivo
	distx = (larg - 350) / 2;					//Centraliza a figura no meio horizontalmente
	disty = (alt - 191) / 2;					//Centraliza a figura no meio horizontalmente
	for(int i = disty; i < alt - disty; i++) {			
		for(int j = distx; j < larg - distx; j++) {
			arqentrada >> valor;
			aux[i][j] = valor;					//Coloca a figura do batman no centro da matriz auxiliar
		}	
	}
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			if(aux[i][j] == 255) {				//Onde na imagem do batman estiver branca 
				v = (int)img[i][j];				//O valor do pixel da imagem a ser gravada é invertida
				v = 255 - v;					//Pixel recebe o valor oposto
				img[i][j] = (unsigned char)v;
			}

		}
	}
}

void clarear_img(unsigned char img[][700], int alt, int larg) {
	int fator, v;
	do {
		cout << "Qual o fator de clareamento (1-100)? ";
		cin >> fator;
	} while((fator < 0) || (fator > 100));

	//*** Clareia a imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			v = (int)img[i][j];					//pega o valor do pixel
			v += fator;							//clareia o pixel
			if(v > 255)							//se der maior que o limite
				v = 255;						//deixa branco
			img[i][j] = (unsigned char)v;		//modifica o pixel
		}
	}
	//*************************//
}

void convergir_img(unsigned char img[][700], int alt, int larg) {

	//*** espelha o lado direito ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg / 2; j++) {			//Roda o laco ate a metade da linha
			img[i][j] = img[i][larg - 1 - j];		//j recebe a posicao da linha que corresponde ao (tamanho da dimensao - a posicao em que j esta no loop - 1)
		}
	}
	//*************************//

}

void divergir_img(unsigned char img[][700], int alt, int larg) {
	//*** espelha o lado esquerdo***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg / 2; j++) {		//Roda o laco ate a metade da linha
			img[i][larg - 1 - j] = img[i][j];	//(tamanho da dimensao - a posicao em que j esta no loop - 1) recebe a posicao da linha que corresponde ao j
		}
	}
	//*************************//
}

void escurecer_img(unsigned char img[][700], int alt, int larg) {

	int fator, v;
	do {
		cout << "Qual o fator de clareamento (1-100)? ";
		cin >> fator;
	} while((fator < 0) || (fator > 100));

	//*** escurece a imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			v = (int)img[i][j];						//pega o valor do pixel
			v -= fator;								//escurece o pixel
			if (v < 0)								//se der negativo
				v = 0;								//  deixa preto
			img[i][j] = (unsigned char)v;			//modifica o pixel
		}
	}
	//*************************//
}

void espelhar_img(unsigned char img[][700], int alt, int larg) {

	int v;

	//*** inverte as linhas da imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg / 2; j++) {				//Roda o laco ate a metade da linha
			v = (int)img[i][j];							//v recebe o valor contido na linha i coluna j
			img[i][j] = img[i][larg - 1 - j];			//o valor contido na coluna da linha i e substituido por (tamanho da dimensao - a posicao em que j esta no loop - 1)
			img[i][larg - 1 - j] = (unsigned char)v;	//(tamanho da dimensao - a posicao em que j esta no loop - 1) recece v
		}
	}
	//*************************//
}

void laplace_img(unsigned char img[][700], int aux[][700], int alt, int larg) {

	int v;
	//Aplica a mascara de Laplace//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			v = ((int)img[i][j]) * 4 + ((int)img[i][j - 1]) * -1 + ((int)img[i][j + 1]) * -1 + ((int)img[i + 1][j]) * -1 + ((int)img[i - 1][j]) * -1;	//aplica a mascara de combinação do valor do pixel com seus vizinhos
			if(v > 255)				//Caso ultrapasse o valor maximo e substituido por ele
				v = 255;
			if(v < 0)
				v = 0;				//Caso ultrapasse o valor minimo e substituido por ele
			aux[i][j] = v;

		}
	}
	//*************************//
	for(int i = 0; i < alt; i++)
		for(int j = 0; j < larg; j++)
			img[i][j] = (unsigned char) aux[i][j];		//Valores da matriz auxiliar são passados para a matriz que gerara a nova imagem

}

void mescla_img(unsigned char img[][700], int alt, int larg) {

	int v, desct = 10, cont = 0;

	///***Realiza a sobreposicao de negativos***//
	do {
		for(int i = desct; i < alt - desct; i++) {				//a cada laço completo tratamos de uma area menor (-10 pixels por laco)
			for(int j = desct; j < larg - desct; j++) {
				v = (int)img[i][j];
				v = 255 - v;								//Pixel recebe o valor oposto
				img[i][j] = (unsigned char)v;
			}
		}
		desct += 10;
		cont++;
	} while(cont < 20);											//O laco sera realizado 20 vezes
	//*************************//
}

void negativo_img(unsigned char img[][700], int alt, int larg) {
	int v;
	//*** inverte as cores da imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			v = (int)img[i][j];
			v = v - 255;					//Pixel recebe o valor oposto
			img[i][j] = (unsigned char)v;
		}
	}
	//*************************//
}

void pretoebranco_img(unsigned char img[][700], int alt, int larg) {

	int v;

	//Aplica os valores extremos na imagem//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			v = (int)img[i][j];
			if(v > 130)			//Caso o pixel seja maior que 130
				v = 255;		//Recebe branco
			else
				v = 0;			//Caso contrario recebe preto
			img[i][j] = (unsigned char)v;
		}
	}
	//*************************//
}

void sobel_img(unsigned char img[][700], int aux[][700], int alt, int larg) {

	int m1, m2, v;									//Auxiliares

	for(int i = 1; i < alt - 1; i++) {				//Lemos 2 pixels a menos na largura
		for(int j = 1; j < larg - 1; j++) {			//Lemos 2 pixels a menos de altura
			m1 = ((int)img[i - 1][j - 1]) * 1 + ((int)img[i - 1][j]) * 2 + ((int)img[i - 1][j + 1]) * 1 + ((int)img[i + 1][j - 1]) * -1 + ((int)img[i + 1][j]) * -2 + ((int)img[i + 1][j + 1]) * -1;			//Varemos os valores em volta do pixel 
			m2 = ((int)img[i][j - 1]) * 2 + ((int)img[i - 1][j - 1]) * 1 + ((int)img[i + 1][j - 1]) * 1 + ((int)img[i - 1][j + 1]) * -1 + ((int)img[i][j + 1]) * -2 + ((int)img[i + 1][j + 1]) * -1;			//Varemos os valores em volta do pixel 
			v = sqrt(pow(m1, 2) + pow(m2, 2));		//Tiramos a media dos dois valores
			//v = (m1 + m2) / 2;
			if(v > 255)								//Se o valor ultrapassar o maximo, recebe o maximo
				v = 255;
			if(v < 0)								//Se o valor ultrapassar o minimo, recebe o minimo
				v = 0;
			aux[i][j] = (unsigned char)v;
		}
	}
	for(int i = 0; i < alt; i++)
		for(int j = 0; j < larg; j++)
			img[i][j] = aux[i][j];					//substitui pelos novos valores na imagem a ser gravada
}

//Colorido

void batman_img(unsigned char img[][700][3], int aux[][700], int alt, int larg) {
	ifstream arqentrada;									//Auxiliares
	int valor, distx, disty, v;								//Auxiliares
	arqentrada.open("batman.pnm", ios::in);					//Abre os valores da matriz de duas dimensões contidas no arquivo
	distx = (larg - 350) / 2;								//Centraliza a figura no meio horizontalmente
	disty = (alt - 191) / 2;								//Centraliza a figura no meio horizontalmente
	for(int i = disty; i < alt - disty; i++) {
		for(int j = distx; j < larg - distx; j++) {
			arqentrada >> valor;
			aux[i][j] = valor;								//Coloca a figura do batman no centro da matriz auxiliar
		}
	}
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			for(int k = 0; k < 3; k++) {
				if(aux[i][j] == 255) {						//Onde na imagem do batman estiver branca 
					v = (int)img[i][j][k];					//O valor do pixel da imagem a ser gravada é invertida
					v = 255 - v;							//Pixel recebe o valor oposto
					img[i][j][k] = (unsigned char)v;
				}
			}
		}
	}
}

void clarear_img(unsigned char img[][700][3], int alt, int larg) {

	int fator, v;

	do {
		cout << "Qual o fator de clareamento (1-100)? ";
		cin >> fator;
	} while((fator < 0) || (fator > 100));

	//*** Clareia a imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			for(int k = 0; k < 3; k++) {
				v = (int)img[i][j][k];				//pega o valor do pixel
				v += fator;							//clareia o pixel
				if(v > 255)							//se der maior que o limite
					v = 255;						//deixa branco
				img[i][j][k] = (unsigned char)v;	//modifica o pixel
			}
		}
	}
	//*************************//
}

void convergir_img(unsigned char img[][700][3], int alt, int larg) {

	//*** espelha o lado direito ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg / 2; j++) { 				//Roda o laco ate a metade da linha
			for(int k = 0; k < 3; k++) {
				img[i][j][k] = img[i][larg - 1 - j][k];		//j recebe a posicao da linha que corresponde ao (tamanho da dimensao - a posicao em que j esta no loop - 1)
			}
		}
	}
	//*************************//
}

void divergir_img(unsigned char img[][700][3], int alt, int larg) {

//*** espelha o lado esquerdo ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg / 2; j++) {					//Roda o laco ate a metade da linha
			for(int k = 0; k < 3; k++) {
				img[i][larg - 1 - j][k] = img[i][j][k];		//(tamanho da dimensao - a posicao em que j esta no loop - 1) recebe a posicao da linha que corresponde ao j
			}

		}
	}
	//*************************//
}

void escurecer_img(unsigned char img[][700][3], int alt, int larg) {

	int fator, v;

	do {
		cout << "Qual o fator de escurecimento (1-100)? ";
		cin >> fator;
	} while((fator < 0) || (fator > 100));

	//*** Escurece a imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			for(int k = 0; k < 3; k++) {
				v = (int)img[i][j][k];				//pega o valor do pixel
				v -= fator;							//escurece o pixel
				if(v < 0)							//se der negativo
					v = 0;							//  deixa preto
				img[i][j][k] = (unsigned char)v;	//modifica o pixel
			}
		}
	}
	//*************************//
}

void espelhar_img(unsigned char img[][700][3], int alt, int larg) {

	int v;

	//*** inverte as linhas da imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg / 2; j++) {					//Roda o laco ate a metade da linha
			for(int k = 0; k < 3; k++) {
				v = (int)img[i][j][k];						//v recebe o valor contido na linha i coluna j
				img[i][j][k] = img[i][larg - 1 - j][k];		//o valor contido na coluna da linha i e substituido por (tamanho da dimensao - a posicao em que j esta no loop - 1)
				img[i][larg - 1 - j][k] = (unsigned char)v; //(tamanho da dimensao - a posicao em que j esta no loop - 1) recece v

			}
		}
	}
	//*************************//
}

void laplace_img(unsigned char img[][700][3], int aux[][700][3], int alt, int larg) {
	int v;

	//Aplica a mascara de Laplace//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			for(int k = 0; k < 3; k++) {
				v = ((int)img[i][j][k]) * 4 + ((int)img[i][j - 1][k]) * -1 + ((int)img[i][j + 1][k]) * -1 + ((int)img[i + 1][j][k]) * -1 + ((int)img[i - 1][j][k]) * -1;	//aplica a mascara de combinação do valor do pixel com seus vizinhos
				if(v > 255) 			//Caso ultrapasse o valor maximo e substituido por ele
					v = 255;
				if(v < 0)
					v = 0;				//Caso utrapasse o valor minimo  e substituido por ele
				aux[i][j][k] = v;
			}
		}
	}
	//*************************//
	for(int i = 0; i < alt; i++)
		for(int j = 0; j < larg; j++)
			for(int k = 0; k < 3; k++)
				img[i][j][k] = (unsigned char) aux[i][j][k];	//Valores da matriz auxiliar são passados para a matriz que gerara a nova imagem

}

void mescla_img(unsigned char img[][700][3], int alt, int larg) {

	int v, desct = 10, cont = 0;

	//***Realiza a sobreposicao de negativos***//
	do {
		for(int i = desct; i < alt - desct; i++) {			//a cada laço completo tratamos de uma area menor (-10 pixels por laco)
			for(int j = desct; j < larg - desct; j++) {
				for(int k = 0; k < 3; k++) {
					v = (int)img[i][j][k];
					v = 255 - v;							//Pixel recebe o valor oposto
					img[i][j][k] = (unsigned char)v;
				}
			}
		}
		//*************************//
		desct += 10;
		cont++;
	} while(cont < 20);										//O laco sera realizado 20 vezes
}

void negativo_img(unsigned char img[][700][3], int alt, int larg) {
	int v;
	//*** inverte as cores da imagem ***//
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			for(int k = 0; k < 3; k++) {
				v = (int)img[i][j][k];
				v = 255 - v;		//Pixel recebe o valor oposto
				img[i][j][k] = (unsigned char)v;
			}
		}
	}
	//*************************//
}

void tonsdecinza_img(unsigned char img[][700][3], unsigned char nova_img[][700], int alt, int larg, string & tipo) {
	int media = 0;									//Auxiliar 
	tipo = "P2";									//Tipo P3 passa a ser P2, para ser gravada em uma matriz de duas dimensoes
	for(int i = 0; i < alt; i++) {
		for(int j = 0; j < larg; j++) {
			for(int k = 0; k < 3; k++) {
				media += (int)img[i][j][k];			//soma os valores RGB do pixel
			}
			media /= 3;								//Tiramos a media
			nova_img[i][j] = (unsigned char)media;	//O valor e aplicado na matriz de duas dimensoes
			media = 0;								//Media e zerada
		}
	}
}

void sobel_img(unsigned char img[][700][3], int aux[][700][3], int alt, int larg) {

	int m1, m2, v;									//Auxiliares

	for(int i = 1; i < alt - 1; i++) {				//Lemos 2 pixels a menos na largura
		for(int j = 1; j < larg - 1; j++) {			//Lemos 2 pixels a menos de altura 
			for(int k = 0; k < 3; k++) {
				m1 = ((int)img[i - 1][j - 1][k]) * 1 + ((int)img[i - 1][j][k]) * 2 + ((int)img[i - 1][j + 1][k]) * 1 + ((int)img[i + 1][j - 1][k]) * -1 + ((int)img[i + 1][j][k]) * -2 + ((int)img[i + 1][j + 1][k]) * -1;		//Varemos os valores em volta do pixel 
				m2 = ((int)img[i][j - 1][k]) * 2 + ((int)img[i - 1][j - 1][k]) * 1 + ((int)img[i + 1][j - 1][k]) * 1 + ((int)img[i - 1][j + 1][k]) * -1 + ((int)img[i][j + 1][k]) * -2 + ((int)img[i + 1][j + 1][k]) * -1;		//Varemos os valores em volta do pixel 
				v = sqrt(pow(m1, 2) + pow(m2, 2));	//Tiramos a media dos dois valores
				//v = (m1 + m2) / 2;
				if(v > 255)							//Se o valor ultrapassar o maximo, recebe o maximo
					v = 255;
				if(v < 0)							//Se o valor ultrapassar o minimo, recebe o minimo
					v = 0;
				aux[i][j][k] = v;
			}
		}
	}
	for(int i = 0; i < alt; i++)
		for(int j = 0; j < larg; j++)
			for(int k = 0; k < 3; k++)
				img[i][j][k] = (unsigned char)aux[i][j][k];		//substitui pelos novos valores na imagem a ser gravada
}
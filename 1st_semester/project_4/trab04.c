#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void freeM(double **matrix, int n){
    int i;                                                                          //Fun��o: freeM(double **, int)
    for( i = 0 ; i < n; i++)                                                        //- Respons�vel por liberar a mem�ria alocada para
        free(matrix[i]);                                                            //matrizes criadas anteriormente...

    return;
}

void Major_Minor(double *major, double *minor, double **img_matrix, int row, int col){
    int i, j;                                                                       //Fun��o: Major_Minor(double *, double *, double **
    for(i = 0 ; i < row ; i++){                                                     //int, int)
        for(j = 0 ; j < col ; j++){                                                 //- Respons�vel por analizar uma matriz e retornar
            if(*major < img_matrix[i][j])                                           //o valor maximo e minimo que existe nela...
                *major = img_matrix[i][j];
            if(*minor > img_matrix[i][j])
                *minor = img_matrix[i][j];
        }
    }

    return;
}

double **createM(int row, int col){
    int i;                                                                          //Fun��o: createM(int, int)
    double **matrix = (double **)calloc(row,sizeof(double *));                      //- Respons�vel por criar uma matriz na mem�ria
        for(i = 0 ; i < row ; i++)                                                  //para o usu�rio com as dimens�es escolhidas por
            matrix[i] = (double *)calloc(col,sizeof(double));                       //ele...

    return matrix;
}

double Convolution_Pixel(double **aux_img_matrix, double **filter_matrix, int d, int add, int row, int col){
    double sum = 0;                                                                 //Fun��o: Convolution_Pixel(double **, double **,
    int i, j;                                                                       //int, int, int, int)
                                                                                    //- Respons�vel por realizar a convolu��o de
    for( i = row ; i < row + d ; i++){                                              //duas matrizes - Observe que ele retorna
        for(j = col ; j < col + d ; j++){                                           //o valor de pixel por pixel da matriz imagem
            sum += (int)(aux_img_matrix[i][j]*filter_matrix[d+row-1-i][d+col-1-j]); //convulucionada, cabendo ao usuario definir
        }                                                                           //o ponto de inicio e parada da convolu��o...
    }

    return sum;
}

char *readFileInfo(FILE *fp){
    char c;                                                             //Fun��o: readFileInfo(FILE *)
    char *string = NULL;                                                //- Respons�vel por realizar a leitura de um arquivo
    int counter = 0;                                                    //(no caso, um arquivo de imagem), analisando se existe
    do{                                                                 //algum coment�rio ap�s a leitura do tipo deste e retornar
        c = fgetc(fp);                                                  //ao usu�rio uma string que cont�m somente o tipo do arquivo...
        string = (char *)realloc(string, sizeof(char)*(counter+1));
        string[counter++] = c;
    }while(c != 10);
    string[counter] = '\0';

    c = fgetc(fp);
    if(c == '#'){
        do{
            c = fgetc(fp);
        }while(c != 10);
    } else ungetc(c,fp);

    string = (char *)realloc(string, sizeof(char)*(counter+1));
    string[counter] = '\0';

    return string;
}

char *readLine(){
    char c;                                                             //Fun��o: readLine()
    char *string = NULL;                                                //- Respons�vel por ler uma linha na entrada padr�o
    int counter = 0;                                                    //de texto at� a entrada de um 'ENTER', retornando
    do{                                                                 //ao usu�rio uma 'string'...
        c = fgetc(stdin);
        string = (char *)realloc(string, sizeof(char)*(counter+1));
        string[counter++] = c;
    }while(c != 10);
    string[counter - 1] = '\0';

    return string;
}

FILE *open2write(){
    FILE *fp = fopen("saida.pgm","w+");                                 //Fun��o: open2write()
        if(fp == NULL){                                                 //- Respons�vel por abrir um arquivo para escrita,
            printf("Invalid write_file.\n");                            //trunca-lo (caso j� exista) e analisar se esse �
            exit(1);                                                    //um arquivo � valido...
        }

    return fp;
}

FILE *open2read(){
    char *file_name = readLine();                                       //Fun��o: open2read()
    FILE *fp = fopen(file_name,"r");                                    //- Respons�vel por abrir um arquivo para leitura
        if(fp == NULL){                                                 //e analisar se esse arquivo � valido...
            printf("There is no file to read.\n");
            free(file_name);
            exit(1);
        }
    free(file_name);

    return fp;
}

void Spatial_Filter(){
//(2) FILTRAGEM ESPACIAL DE UMA IMAGEM
    int i, j;

    //A.1 ABERTURA DO ARQUIVO COM A IMAGEM PARA LEITURA...                                          //-> A.1. Ao se iniciar o processo de filtragem espacial de uma imagem, em primeiro
    FILE *img_file = open2read();                                                                   //lugar, � aberto o arquivo com a imagem na qual se deseja operar...

    //A.2 ABERTURA DE UM ARQUIVO PARA A ESCRITA (SAIDA)...                                          //-> A.2. Ap�s a abertura do 'arquivo de entrada', cria-se o arquivo de saida que
    FILE *spatial_file = open2write();                                                              //ir� conter a imagem ap�s a passagem do filtro..

    //B. ABRINDO O ARQUIVO COM O FILTRO PARA LEITURA...                                             //-> B. O filtro � escolhido a partir da abertura do arquivo que contem as informa��es
    FILE *filter_file = open2read();                                                                //da matriz do filtro...

    //C. CRIANDO UMA MATRIZ COM AS INFORMA��ES DO FILTRO LIDO...
    int dimension;                                                                                  //-> C. Com o filtro escolhido, s�o extraidos do arquivo deste as principais informa��es
    fscanf(filter_file,"%d",&dimension);                                                            //de sua matriz, assim como � criada uma matriz de uso atrav�s da leitura do arquivo...

    double **filter_matrix = createM(dimension,dimension);
    for(i = 0 ; i < dimension ; i++)
        for(j = 0 ; j < dimension ; j++)
            fscanf(filter_file,"%lf",&filter_matrix[i][j]);

    //D. CRIANDO UMA MATRIZ COM AS INFORMA��ES DA IMAGEM LIDA...
    int col, row, max_intense, add = dimension/2;                                                   //-> D. O mesmo � feito com a matriz da imagem: extrai-se as principais informa��es de sua
    char *type = readFileInfo(img_file);                                                            //matriz, assim como � criada uma matriz de uso atrav�s da leitura do arquivo...
        fscanf(img_file,"%d %d %d",&col,&row,&max_intense);                                         //- Observe, por�m, que a matriz criada a partir da leitura do arquivo da imagem possui um
                                                                                                    //n�mero de colunas e linhas adicionais em compara��o com a matriz original.
    double **aux_img_matrix = createM(row+(2*add),col+(2*add));                                     //- Isto � feito para que seja poss�vel a realiza��o da 'convolu��o de matrizes' de maneira
    for( i = 0 ; i < (row+(add*2)) ; i++ ){                                                         //mais direta e simples...
        for( j = 0 ; j < (col+(add*2)) ; j++ ){
            if(i >= add && j >= add && i < row+add && j < col+add)
                fscanf(img_file,"%lf",&aux_img_matrix[i][j]);
        }
    }

    //E. REALIZANDO A CONVOLU��O DE MATRIZ...
    double **img_matrix = createM(row,col);                                                         //-> E. Aqui � realizada a convolu��o da matriz auxiliar criada no passo D com a matriz
    for( i = 0 ; i < row ; i++ ){                                                                   //do filtro, lida e criada no passo B e C, respectivamente...
        for( j = 0 ; j < col ; j++ ){                                                               //- Observe que os valores adquiridos na convolu��o destas s�o guardados na matriz imagem
            img_matrix[i][j] = Convolution_Pixel(aux_img_matrix,filter_matrix,dimension,add,i,j);   //que contem as mesmas caracteristicas (exceto pelos valores de seus pontos) que a matriz
        }                                                                                           //da imagem original...
    }

    //F. DESCOBRINDO OS MINIMOS E MAXIMOS...
    double gmax = img_matrix[0][0], gmin = img_matrix[0][0];                                        //-> F. Neste passo, s�o descobertos os valores maximos e minimos de intesidade luminosa
    Major_Minor(&gmax,&gmin,img_matrix,row,col);                                                    //atribuida a matriz da imagem ap�s a convulu��o da matriz auxiliar com a matriz filtro...

    //G. NORMALIZANDO OS VALORES DA MATRIZ...                                                       //-> G. Com o ponto maximo e minimo de luminosidade bem definidos, normaliza-se a matriz
    for( i = 0 ; i < row ; i++ )                                                                    //resultante a fim de manter a mesma caracteristica de luminosidade maxima da imagem
        for( j = 0 ; j < col ; j++ )                                                                //original aberta anteriormente...
            img_matrix[i][j] = (int)(max_intense*((img_matrix[i][j] - gmin)/(gmax-gmin)));

    //H. IMPRIMINDO NA TELA AS INFOR��ES E CRIANDO UM ARQUIVO COM O RESULTADO...
    printf("%s%d %d\n%d\n",type,col,row,max_intense);
    fprintf(spatial_file,"%s%d %d\n%d\n",type,col,row,max_intense);                                 //-> H. Por fim, s�o impressos na tela e gravados em um arquivo de sa�da as informa��es
    for( i = 0 ; i < row ; i++ ){                                                                   //da imagem criada a partir da passagem do filtro espacial...
        for( j = 0 ; j < col ; j++ ){
            printf("%d ",(int)img_matrix[i][j]);
            fprintf(spatial_file,"%d ",(int)img_matrix[i][j]);
        }printf("\n");
         fputs("\n",spatial_file);
    }

    //I. LIBERANDO A MEMORIA ALOCADA E FECHANDO OS ARQUIVOS ABERTOS...
    fclose(img_file);                                                                               //-> I. E libera-se toda a memoria alocada no processo de filtragem espacial, al�m de
    fclose(filter_file);                                                                            //se fechar os arquivos abertos...
    freeM(filter_matrix,dimension);
    free(filter_matrix);
    free(type);
    freeM(aux_img_matrix,(row+(add*2)));
    free(aux_img_matrix);
    freeM(img_matrix,row);
    free(img_matrix);

    return;
}

void Negative_Img(){
//(1) PRODU��O DO NEGATIVO DE UMA IMAGEM
    int i, j;

    //A. ABERTURA DO ARQUIVO COM A IMAGEM PARA LEITURA...                                           //-> A. Ao se iniciar o processo de produ��o de uma imagem negativa, em primeiro
    FILE *img_file = open2read();                                                                   //lugar, � aberto o arquivo com a imagem na qual se deseja operar...

    //B. CRIA��O DE UM ARQUIVO DE SAIDA...                                                          //-> B. Ap�s a abertura do 'arquivo de entrada', cria-se o arquivo de saida que
    FILE *negative_img = open2write();                                                              //ir� conter a imagem negativa...

    //C. OBTENDO INFORMA��ES IMPORTANTES DA 'MATRIZ DA IMAGEM'...
                                                                                                    //-> C. Com os arquivos j� abertos, s�o extra�das as principais informa��es da matriz
    int pixel, col, row, max_intense;                                                               //imagem contidas nos arquivos - numero de linhas e colunas da matriz, intensidade lu_
    char *type = readFileInfo(img_file);                                                            //minosa m�xima da imagem e o tipo da imagem...

    fscanf(img_file,"%d %d %d",&col,&row,&max_intense);

    //D. CRIA��O DA IMAGEM NEGATIVA...
    printf("%s%d %d\n%d\n",type,col,row,max_intense);                                               //-> D. Conhecendo as principais informa��es da imagem, come�a-se de fato a produ��o
    fprintf(negative_img,"%s%d %d\n%d\n",type,col,row,max_intense);                                 //da imagem negativa que � obtida atrav�s da mudan�a dos valores dos 'pontos' em sua
    for(i = 0 ; i < row ; i++){                                                                     //matriz principal (matriz da imagem)...
        for(j = 0 ; j < col ; j++){                                                                 //- Observe que o tipo, as dimens�es e a intensidade luminosa maxima s�o iguais na
            fscanf(img_file,"%d",&pixel);                                                           //imagem de sa�da e de entrada..
            printf("%d ",max_intense - pixel);
            fprintf(negative_img,"%d ",max_intense - pixel);
        }printf("\n");
         fputs("\n",negative_img);
    }

    //E. LIBERANDO A MEMORIA ALOCADA E FECHANDO OS ARQUIVOS...
    fclose(img_file);                                                                               //-> E. Com o arquivo de sa�da completo e com as informa��es da imagem impressas na
    fclose(negative_img);                                                                           //tela, s�o liberadas as memorias alocadas e fechados os arquivos abertos anteriormente...
    free(type);

    return;
}

int main ( int argc, char **argv ){
    //1. ESCOLHENDO A OPERA��O A SER REALIZADA...
    int select_op = 0;                                                                            //1) Inicialmente, pede-se ao usu�rio que
    while( select_op != 1 && select_op != 2 && select_op != -1){                                  //entre com o valor que representa a opera��o
        scanf("%d%*c",&select_op);                                                                //que deseja realizar, sendo 1 para negativar
        if(select_op != 1 && select_op != 2 && select_op != -1){                                  //a imagem e 2 para realizar um filtro espacial.
            printf("Invalid command. Reenter the operation's number.\n");
            printf("-Usage: 1 (for 'negative_image) / 2 (for 'spatial filter'); / -1 (for exit).\n\n");
        }
    }

    //2. INICIANDO O PROCESSO ESCOLHIDO...
    if(select_op == 1) Negative_Img();                                                            //2)Ap�s a entrada da opera��o inicia-se o processo escolhido atrav�s da
    else if(select_op == 2) Spatial_Filter();                                                     //analise do valor atribuido � vari�vel 'op'...
                                                                                                  //- O programa � encerrado quando termina-se a opera��o escolhida, quando
                                                                                                  //ocorre algum erro ou quando 'op = -1';
    return 0;
}

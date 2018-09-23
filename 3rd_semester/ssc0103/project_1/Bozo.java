package poo;

public class Bozo {
	
	public static final int INICIO = 0;
	public static final int JOGAR = 1;
	public static final String JOGAR_MSG = "\n:: Jogar - Bozo ::";
	public static final int REGRAS = 2;
	public static final String REGRAS_MSG = "\n|:: Regras - Bozo ::";
	public static final int SAIR = 3;
	public static final String SAIR_MSG = "\n:: Obrigrado por utilizar nosso programa. Ate a proxima! ::";
	
	static void printBoasVindas(){
		System.out.println("----------\tBOZO\t----------");
		System.out.println("Bem vindo ao ambiente de jogo de Bozo!");
		return;
	}
	
	static void printRegrasBozo(){
		System.out.println("| O Bozo e um jogo de  dados. Cada jogador tem que tentar ");
		System.out.println("| preencher 10 posi��es de um tabuleiro.");
		System.out.println("| Em cada rodada, o  jogador  lan�a os 5 dados. Depois po_");
		System.out.println("| de escolher ficar com a  pontua��o  que  tirou  ou esco_");
		System.out.println("| lher entre 1 e 5 dados para lan�ar  novamente.");
		System.out.println("| Em seguida pode, se quiser, escolher  mais  uma vez  al_");
		System.out.println("| guns dos dados para lan�ar de novo. ");
		System.out.println("| Ao final, deve escolher qual a posi��o  do tabuleiro ira");
		System.out.println("| preencher. Uma posi��o ja preenchida em rodada anterior,");
		System.out.println("| n�o pode ser utilizada de novo. A pontua��o  obtida  de_");
		System.out.println("| pende da posi��o escolhida e da configuracao  final  dos");
		System.out.println("| dados.");
		System.out.println("| Nas posicoes externas devem ser colocadas as  somas  dos");
		System.out.println("| que apreesentam os numeros correspondentes a posicao es_");
		System.out.println("| colhida. Por exeplo, se o  jogado  seleciona  a  posi��o");
		System.out.println("| �# 3� e nos dados consguiu tirar 4 dados com a pontua��o"); 
		System.out.println("| 3, ent�o o valor a ser colocado e 12.");
		System.out.println("| Nas posi��es internas, o jogador recebe uma pontua��o fi_");
		System.out.println("| xa se conseguir determina configura��o dos dados e   zero");
		System.out.println("| se os dados n�o apresentarem tal configura��o. A primeira");
		System.out.println("| posi��o � um full hand ou seja, tr�s dados de determinado");
		System.out.println("| n�mero e dois de outro (ou do mesmo) e vale 15 pontos. A ");
		System.out.println("| segunda � uma sequ�ncia, ou seja, os dados t�m  todos  os");
		System.out.println("| n�meros de 1 a 5 ou de 2 a  6, valendo 20 pontos. A  ter_");
		System.out.println("| ceira � a posi��o da quadra (30 pontos) e a �ltima da qui_");
		System.out.println("| na, que vale 40 pontos.\n");
		return;
	}
	
	static void jogadaExtra(int n, RolaDados dados){
		//Este metodo realiza as rodadas extras do Bozo
		for(int extra = n ; extra > 0 ; extra--){
			java.lang.String str = null;
			System.out.printf("|\tVoce tem %d jogadas extras\t|\nDigite os numeros dos dados que pretende rolar novamente...\n",extra);
			try{str = EntradaTeclado.leString();}catch(Exception e){return;}
			
			dados.rolar(str);
			System.out.println(dados.toString());		
		}
		return;
	}
	
	static void jogarBozo(){
		//Variaveis
		int pos = 0;
		RolaDados dados = new RolaDados(5);
		Placar tab = new Placar();
		
		//Iniciando o jogo de Bozo
		for(int rodada = 0 ; rodada < 10 ; rodada++){
			//1. Todo inicio de rodada os 5 dados sao lancados...
			System.out.printf("|\tRodada %d\t|\n>>Aperte 'ENTER' para lancar os 5 dados iniciais<<",rodada+1);
			try{EntradaTeclado.leString();}catch(Exception e){}
			
			//a. Rolando todos os dados...
			dados.rolar();
			System.out.println(dados.toString());
			
			//b. Realizando as jogadas extras - mudar dados ou parar...
			jogadaExtra(2,dados);
			
			//c. Preenchendo a tabela de pontos/placar...
			pos = 0;
			while(pos == 0){
				System.out.printf("Escolha qual posicao do tabuleiro voce ira preencher.\n\n");
				System.out.println(tab.toString());
				try{pos = EntradaTeclado.leInt();}
				catch(Exception e){System.out.println(":: Posicao invalida ::");}
			}
			tab.add(pos,dados.getDados());
			System.out.printf(":: RESULTADO RODADA %d ::\n",rodada+1);
			System.out.println(tab.toString());
		}
		System.out.printf("| FIM DE JOGO - RESULTADO |\n");
		System.out.println(tab.toString());
		System.out.printf("Voce marcou %d pontos!\n- Ate o proximo jogo!\n\n",tab.getScore());
		return;
	}
	
	public static void	main(String[] args){
		//Variaveis Gerais
		int operacao = Bozo.INICIO;
		
		//1. Comecando o jogo...
		Bozo.printBoasVindas();
		
		//2. Rodando o programa enquanto o usuario nao desejar sair...
		while(operacao != Bozo.SAIR){
			//a. Lendo a operacao que o usuario deseja realizar...
			System.out.printf("Comandos:\n|1-Jogar     2-Regras     3-Sair|\n");
			try{
				operacao = EntradaTeclado.leInt();
				if(operacao < 1 || operacao > 3)
					System.out.println("O comando entrado e invalido. Tente outro!\n");
			} catch(Exception e){
				System.out.println("O comando entrado e invalido. Tente outro!\n");
				operacao = Bozo.INICIO;
			}
			
			//b. Inicializando a operacao selecionada...
			switch(operacao){
				case Bozo.JOGAR:
					System.out.println(Bozo.JOGAR_MSG);	
					jogarBozo();
					break;
				case Bozo.REGRAS:
					System.out.println(Bozo.REGRAS_MSG);
					printRegrasBozo();
					break;
				case Bozo.SAIR:
					System.out.println(Bozo.SAIR_MSG);
					break;
			}
			//c. Caso a operacao nao seja SAIR, o programa continua rodando...
		}
		
		//3. Encerrando programa....
		return;
	}
	
}

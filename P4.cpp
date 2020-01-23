#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <math.h>

using namespace std;

class P4{
public:

	vector<vector<short>> board;//Plateau de jeu

	P4(int width, int height){//Constructeur
		vector<short> v(height, 0);
		for(int i = 0 ; i < width ; i++)
			board.push_back(v);
	}

	P4(const P4 &old_obj){//Constructeur de copie
		board = old_obj.board;
	}

	void print(){//Affichage du plateau de jeu
		for(int i=board[0].size() - 1 ; i >= 0 ; i--){
			for(auto v : board){
				if(v[i] == 1)
					cout << "\033[31m◉ \033[0m";
				else if(v[i] == -1)
					cout << "\033[33m◉ \033[0m";
				else
					cout << "○ ";

			}
			cout << endl;
		}
		cout << "\033[31m0 1 2 3 4 5 6 7 8 9\033[0m" << endl;
	}

	bool putToken(int player, int x){//Fonction pour placer un jeton; player(1 = Joueur / -1 = Ordinateur)
		for(int i=0 ; i<board[x].size() ; i++){
			if(board[x][i] == 0){
				board[x][i] = player;
				return 1;
			}
		}
		return 0;
	}

	bool is_number(std::string s){//Fonction qui renvoie vrai si la chaîne de caractères passée en paramètre peut être convertie en entier
		auto it = s.begin();
		while (it != s.end() && isdigit(*it)) 
			++it;
		return !s.empty() && it == s.end();
	}

	void playerTurn(){//Fonction qui permet au joueur de saisir la position où jouer le jeton
		string x;
		cout << "Où voulez-vous placer votre pièce ?" << endl;
		cin >> x;
		while(!is_number(x) || stoi(x) > board.size()){
			cout << "Valeur incorrecte" << endl;
			cin >> x;
		}
		if(putToken(1, stoi(x))){
			print();
			if(victory(stoi(x))){
				cout << "Vous avez gagné !" << endl;
				exit(0);
			}
			return;
		}
		cout << "Position invalide" << endl;
		playerTurn();
	}

	bool check_coord(int x, int y){//Vérifie que les coordonnées du jeton sont valides (càd qu'elles se trouvent bien à l'intérieur du plateau) pour éviter d'accéder à une zone mémoire non allouée. 
		if(x >= 0 && x < board.size() && y >= 0 && y < board[0].size()){
			return 1;
		}
		return 0;
	}

	bool victory(int x){//Renvoie vrai en cas de victoire; x correspond à l'abscisse du dernier jeton joué et donc aussi à sa couleur pour savoir qui a gagné. 
		
		int y = -1; //Ordonnée du dernier jeton joué
		for(auto token : board[x]){//Récupère la bonne ordonnée
			if(token == 0)
				break;
			y++; 
		}
		//Chacunes des lignes suivantes testent si il y a alignementde 4 jetons du même joueur pour les 16 combinaisons possibles de puissance 4 autour d'une case précise.
		if( check_coord(x, y+1) && check_coord(x, y+2) && check_coord(x, y+3) 		&& board[x][y+1] == board[x][y] && board[x][y+2] == board[x][y] && board[x][y+3] == board[x][y] ||
			check_coord(x, y-1) && check_coord(x, y+1) && check_coord(x, y+2) 		&& board[x][y-1] == board[x][y] && board[x][y+1] == board[x][y] && board[x][y+2] == board[x][y] ||
			check_coord(x, y-2) && check_coord(x, y-1) && check_coord(x, y+1) 		&& board[x][y-2] == board[x][y] && board[x][y-1] == board[x][y] && board[x][y+1] == board[x][y] ||
			check_coord(x, y-3) && check_coord(x, y-2) && check_coord(x, y-1) 		&& board[x][y-3] == board[x][y] && board[x][y-2] == board[x][y] && board[x][y-1] == board[x][y] ||

			check_coord(x+1, y+1) && check_coord(x+2, y+2) && check_coord(x+3, y+3) && board[x+1][y+1] == board[x][y] && board[x+2][y+2] == board[x][y] && board[x+3][y+3] == board[x][y] ||
			check_coord(x-1, y-1) && check_coord(x+1, y+1) && check_coord(x+2, y+2) && board[x-1][y-1] == board[x][y] && board[x+1][y+1] == board[x][y] && board[x+2][y+2] == board[x][y] ||
			check_coord(x-2, y-2) && check_coord(x-1, y-1) && check_coord(x+1, y+1) && board[x-2][y-2] == board[x][y] && board[x-1][y-1] == board[x][y] && board[x+1][y+1] == board[x][y] ||
			check_coord(x-3, y-3) && check_coord(x-2, y-2) && check_coord(x-1, y-1) && board[x-3][y-3] == board[x][y] && board[x-2][y-2] == board[x][y] && board[x-1][y-1] == board[x][y] ||

			check_coord(x+1, y) && check_coord(x+2, y) && check_coord(x+3, y) 		&& board[x+1][y] == board[x][y] && board[x+2][y] == board[x][y] && board[x+3][y] == board[x][y] ||
			check_coord(x-1, y) && check_coord(x+1, y) && check_coord(x+2, y) 		&& board[x-1][y] == board[x][y] && board[x+1][y] == board[x][y] && board[x+2][y] == board[x][y] ||
			check_coord(x-2, y) && check_coord(x-1, y) && check_coord(x+1, y) 		&& board[x-2][y] == board[x][y] && board[x-1][y] == board[x][y] && board[x+1][y] == board[x][y] ||
			check_coord(x-3, y) && check_coord(x-2, y) && check_coord(x-1, y) 		&& board[x-3][y] == board[x][y] && board[x-2][y] == board[x][y] && board[x-1][y] == board[x][y] ||

			check_coord(x+1, y-1) && check_coord(x+2, y-2) && check_coord(x+3, y-3) && board[x+1][y-1] == board[x][y] && board[x+2][y-2] == board[x][y] && board[x+3][y-3] == board[x][y] ||
			check_coord(x-1, y+1) && check_coord(x+1, y-1) && check_coord(x+2, y-2) && board[x-1][y+1] == board[x][y] && board[x+1][y-1] == board[x][y] && board[x+2][y-2] == board[x][y] ||
			check_coord(x-2, y+2) && check_coord(x-1, y+1) && check_coord(x+1, y-1) && board[x-2][y+2] == board[x][y] && board[x-1][y+1] == board[x][y] && board[x+1][y-1] == board[x][y] ||
			check_coord(x-3, y+3) && check_coord(x-2, y+2) && check_coord(x-1, y+1) && board[x-3][y+3] == board[x][y] && board[x-2][y+2] == board[x][y] && board[x-1][y+1] == board[x][y]) {

			return 1;
		}
		return 0;
	}

	//Renvoie un score en fonction de la couleur des 4 jetons passé en paramètre; x = couleur du jeton par rapport auquel on évalue les trois autres jetons sur un même alignement.
	int heuristicPerRow(short t1, short t2, short t3, int x){ //x (1 = Joueur, -1 = Ordinateur)
		if(t1 == 0 && t2 == 0 && t3 == 0)
			return 1;//Si que des cases vides
		if(t1 == x && t2 == 0 && t3 == 0 || t1 == 0 && t2 == x && t3 == 0 || t1 == 0 && t2 == 0 && t3 == x)
			return 4;//Si 1 jeton de la même couleur
		if(t1 == x && t2 == x && t3 == 0 || t1 == x && t2 == 0 && t3 == x || t1 == 0 && t2 == x && t3 == x)
			return 16;//Si 2 jetons de la même couleur
		if(t1 == x && t2 == x && t3 == x)
			return 1000;//Si 3 jetons de la même couleur (donc victoire)

		else
			return 0;//Tous les autres cas
	}

	int heuristic(){//Fonction principale d'évaluation du plateau de jeu
		int sum = 0;//Valeur qui sera renvoyée à la fin en fonction de l'avantage d'un joueur sur l'autre; 0 = égalité parfaite, >0 = Avantage Joueur, <0 = Avantage Ordinateur
		int mode;//1 = Joueur / -1 = Ordinateur
		for(int x=0 ; x < board.size() ; x++){	//Pour chaque colonnes
			for(int y=0 ; y < board.size() ; y++){	//Pour chaque jetons
				if(board[x][y] != 0){
					mode = board[x][y];
					if(check_coord(x, y+1) && check_coord(x, y+2) && check_coord(x, y+3))
						sum += heuristicPerRow(board[x][y+1],board[x][y+2],board[x][y+3], mode) * mode;
					
					if(check_coord(x+1, y+1) && check_coord(x+2, y+2) && check_coord(x+3, y+3))
						sum += heuristicPerRow(board[x+1][y+1],board[x+2][y+2],board[x+3][y+3], mode) * mode;
					
					if(check_coord(x+1, y) && check_coord(x+2, y) && check_coord(x+3, y))
						sum += heuristicPerRow(board[x+1][y],board[x+2][y],board[x+3][y], mode) * mode;
					
					if(check_coord(x+1, y-1) && check_coord(x+2, y-2) && check_coord(x+3, y-3))
						sum += heuristicPerRow(board[x+1][y-1],board[x+2][y-2],board[x+3][y-3], mode) * mode;

					if(check_coord(x, y-1) && check_coord(x, y-2) && check_coord(x, y-3))
						sum += heuristicPerRow(board[x][y-1],board[x][y-2],board[x][y-3], mode) * mode;
					
					if(check_coord(x-1, y-1) && check_coord(x-2, y-2) && check_coord(x-3, y-3))
						sum += heuristicPerRow(board[x-1][y-1],board[x-2][y-2],board[x-3][y-3], mode) * mode;

					if(check_coord(x-1, y) && check_coord(x-2, y) && check_coord(x-3, y))
						sum += heuristicPerRow(board[x-1][y],board[x-2][y],board[x-3][y], mode) * mode;
					
					if(check_coord(x-1, y+1) && check_coord(x-2, y+2) && check_coord(x-3, y+3))
						sum += heuristicPerRow(board[x-1][y+1],board[x-2][y+2],board[x-3][y+3], mode) * mode;
				}
			}
		}
		return sum;
	}

	int minMax(int d, int depth, int start, int end, vector<int>* finalMove){//Fonction principale de recherche en profondeur du meilleur coup possible.
		vector<vector<short>> save = board;//Sauvegarde de l'état du jeu actuel
		vector<int> bestMove;//Vecteur d'entier qui contiendra les évaluations des plateau de jeu, l'index du vecteur correspond à la colonne où le jeton est joué.
		int best;//Pour comparer à la fin les entiers de bestMove et en sortir le plus grand (ou moins grand pour l'ordinateur). 
		int bestPos;//Index où l'entier best à été trouvé dans bestMove qui correspondra à l'endroit ou l'ordinateur devra jouer son jeton à la fin. 

		if(d < depth){//d = profondeur actuelle // depth = profondeur de recherche maximum. 
			if(d){//Si d != 0, pour éviter le cas particulier où on est encore à la première profondeur de recherche (celle où les tâches sont partagées entre les threads). 
				for(int i=0 ; i<board.size() ; i++){//Pour chaque colonnes
					if(d%2 == 0){//Tour de l'ordinateur (car on commence avec d=0 et on commence toujours par un tour de l'ordinateur, donc si d est pair alors on est sur un tour de l'ordinateur).
						if(!putToken(-1, i)){//Simuler le coup pour ensuite évaluer les états suivants. 
							bestMove.push_back(INT32_MAX);//Si le coup n'a pu être joué (si la colonne est déjà pleine), alors assigné à ce coup une valeur très élévé que l'ordinateur ne choisira jamais comme meilleur coup.
							continue;//Coup suivant
						}
					}
					else{//Idem qu'au dessus mais pour simuler un coup du joueur humain. 
						if(!putToken(1, i)){
							bestMove.push_back(INT32_MIN);
							continue;
						}
					}
					bestMove.push_back(minMax(d+1, depth, 0, 0, nullptr));//Appel récursif de la fonction si l'on était pas encore à la profondeur max de recherche. L'appel se fait avec le plateau de jeu modifié et la profondeur actuelle + 1. 
					board = save;//Annuler le coup. 
				}
				//Après avoir parcouru tous les scénarios possibles la fonction renvoie la valeur la + élevée pour simuler un coup du joueur humain où la - élevée pour simuler un coup de l'ordinateur. 
				if(d%2 == 0){//Tour de l'ordinateur
					best = INT32_MAX;//L'ordinateur cherche la valeur la plus basse, donc j'initialise la valeur de retour avec une valeur très élevée
					for(int i=0 ; i<bestMove.size() ; i++){//bestMove.size() == board.size() ici donc peu importe. 
						if(bestMove[i] < best){
							best = bestMove[i];
						}
					}
				}
				else{//Tour du joueur ; Idem qu'au dessus mais en inversant les comparaisons.
					best = INT32_MIN;
					for(int i=0 ; i<bestMove.size() ; i++){
						if(bestMove[i] > best){
							best = bestMove[i];
						}
					}
				}
				return best;
			}
			else{//Première profondeur (celle où on sépare en fonction des threads), c'est toujours le tour de l'ordinateur ici. 
				for(int i=start ; i<end ; i++){//start et end correspondent aux n° des colonnes du plateau. Le thread qui appelle cette fonction ne va évaluer les scénarios possibles qu'à partir des premiers coups qui seront joué entre ces colonnes (start inclu et end exclu).
					if(!putToken(-1, i)){
						bestMove.push_back(INT32_MAX);
						board = save;
						continue;
					}
					bestMove.push_back(minMax(d+1, depth, 0, 0, nullptr));
					board = save;//Annuler le coup
				}
				//Tour de l'ordinateur
				for(int i=0 ; i<bestMove.size() ; i++){//auto x : bestMove){
					(*finalMove)[start + i] = bestMove[i];//Ici au lieu de trouver et retourner la meilleur valeur du meilleur coup possible, on stocke les valeurs dans la bonne portion du vecteur finalMove passé en paramètre en fonction du thread actuel.
				}
				return 0;//La valeur de retour de la fonction est sans importance ici.
			}
		}
		else
			return heuristic();//Renvoie le score d'évaluation du plateau de jeu. 
	}
};

	int threads(int depth, int nbThreads, P4* b){
		vector<int> finalMove(b->board.size());
		vector<thread> v_threads(b->board.size());

		P4 copy = *b;//Copie du plateau de jeu

		for(int i=0 ; i<nbThreads ; i++){//Répartition des colonnes du plateau entre les threads. 

			float s = float(i)*(b->board.size()/float(nbThreads));
			float e = s + b->board.size()/float(nbThreads);
			int start = ceil(s);//Arrondir à l'entier supérieur.
			int end = ceil(e);//Idem

			v_threads[i] = thread(&P4::minMax, copy, 0, depth, start, end, &finalMove);
		}

		for(int i=0 ; i<nbThreads ; i++){//Attente de la fin de chaque threads. 
			v_threads[i].join();
		}

		//Récupération du meilleur coup

		int best = INT32_MAX;
		int bestPos;
		for(int i=0 ; i<finalMove.size() ; i++){
			if(finalMove[i] < best){
				best = finalMove[i];
				bestPos = i;
			}
		}
		return bestPos;//Contient désormais l'index de la colonne où jouer le meilleur coup. 
	}

	void computerTurn(int depth, int nbThreads, P4* b){
		int x = threads(depth, nbThreads, b);
		b->putToken(-1, x);
		b->print();
		if(b->victory(x)){
			cout << "Vous avez perdu !" << endl;
			exit(0);
		}
	}



int main(int argc, char * argv[]){

	if(argc != 5){//Etre sûr qu'il y a bien 4 arguments.
		cout << "Usage : " << argv[0] << " width height numberOfThreads difficulty\n";
		exit(1);
	}

	if(atoi(argv[1]) < 4 || atoi(argv[2]) < 4){
		cout << "width and height must be equal or above 4\n";
		exit(1);
	}

	if(atoi(argv[3]) < 1){
		cout << "numberOfThreads must be equal or above 1\n";
		exit(1);
	}

	if(atoi(argv[1]) < atoi(argv[3])){
		cout << "width must be equal or above numberOfThreads\n";
		exit(1);
	}

	if(atoi(argv[4]) < 1){
		cout << "difficulty must be equal or above 1\n";
		exit(1);
	}


	P4 b(atoi(argv[1]), atoi(argv[2]));

	b.print();

	for(int i=0 ; i< atoi(argv[1]) * atoi(argv[2]) ; i+=2){
		b.playerTurn();
		computerTurn(atoi(argv[4]), atoi(argv[3]), &b);
	}
	cout << "Egalité !" << endl;
}
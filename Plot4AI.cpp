/*
Name:Harsh Shah
Roll no.:200050049
Plot 4 game with AI

Compile using "fltk-config --compile Plot4AI"
Run using "./Plot4"
*/

#include <FL/Fl.H> 
#include <FL/Fl_Window.H> 
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iostream>
#include<ctime>
#include <vector>
#include<fstream>

#define NDEBUG

#ifdef NDEBUG
#define endl file
#endif

class false_fstream{
public:
	false_fstream operator <<(const char* a){
		return *this;
	}
	false_fstream operator <<(int a){
		return *this;
	}
	false_fstream operator <<(false_fstream a){
		return *this;
	}
};


const int x=7;
const int y=7;
const int w=1000,h=800;
const int linewidth=5;
const int topLeftx=100,topLefty=150;
const int board_width=::w-200,board_height=::h-200;
const int xspacing=float(::w-200)/(::x),yspacing=float(::h-200)/(::y);
int moves=0;
int data[7][7];
false_fstream file;


//function to wait for given amount of time
void sleep(float sec){
	clock_t start=clock();
	float wait=sec*CLOCKS_PER_SEC;

	while(clock()<wait+start){;}

	return;
}

class Shape:public Fl_Widget{
public:
	Shape(int x,int y,int w,int h):Fl_Widget(topLeftx,topLefty,w,h){}
	void draw()override{
		fl_draw_box (FL_FLAT_BOX,topLeftx,topLefty,board_width,board_height,50);
		for(int i=0;i<::x+1;i++){
			fl_draw_box (FL_FLAT_BOX,topLeftx+i*xspacing,topLefty,linewidth,board_height,0);
		}
		for(int j=0;j<::y+1;j++){
			fl_draw_box(FL_FLAT_BOX,topLeftx,topLefty+j*yspacing,board_width,linewidth,0);
		}
	}

};

class AI{
	const int depth=8;
	void check3X3(int board[::x][::y],int x,int y);
	int minimax(int board[::x][::y],int depth,bool player);
	void print_board(int board[::x][::y]);
	int checkTotalScore(int board[::x][::y]);

protected:
	int score[2];
	static int ai;
	void check4X4(int board[::x][::y],int x,int y);
	bool checkdraw(int board[::x][::y]);

public:
	AI(){
		score[0]=0;score[1]=0;
	}
	int AImove(int board[::x][::y]);
	int winner(int board[::x][::y]);
};

int AI::ai=0;

int AI::winner(int board[::x][::y]){
	score[0]=0;score[1]=0;
	for(int p=0;p<4;p++){
		for(int q=0;q<4;q++){
			check4X4(board,p,q);
			if(score[0]!=0){
				file<<"pq"<<p<<" "<<q<<endl;
				return 1;
			}else if(score[1]!=0){
				return 2;
			}
		}
	}
	return 0;

}

int AI::AImove(int board[::x][::y]){
	minimax(board,depth,true);
	file<<"AImove returns: "<<this->ai<<endl;
	return ai;
}

class GAME:public AI{
	Shape* outerRectangle;
protected:

	GAME(){
		;//does nothing
	}

public:
	void assign_columns();
	void get_click();
	GAME(bool c);
};


class Button : public Fl_Widget,public GAME{
protected:
	int i,x,y,w,h,c;
public:
		Button (int i, int x,int y, int w, int h) : Fl_Widget (topLeftx+i*xspacing,topLefty,board_width,board_height){
			this->i=i; this->x=x;
			this->w=w; this->h=h;this->y=y;
		}

		virtual void draw()override {/*does nothing*/};

		virtual int handle(int e)override;

		void click_made();

};

void AI::print_board(int board[::x][::y]){
	for(int i=0;i<::x;i++){
		for(int j=0;j<::y;j++){
			file<<board[j][::x-1-i]<<" ";
		}
		file<<endl;
	}
	file<<"***************************\n";
}

int AI::minimax(int board[::x][::y],int depth,bool player){

	if(winner(board)==1){print_board(board);file<<"returned-200\n";return -200;}
	if(winner(board)==2){print_board(board);file<<"returned100\n";return 100;}

	if(depth==0){
		print_board(board);
		int tempResult=checkTotalScore(board);
		return tempResult;
	}


	if(player){
		int maxscore=-1000;
		for(int i=0;i<::x;i++){
			for(int j=0;j<::y;j++){
				if(board[i][j]==0){
					
					board[i][j]=2;
					int result=minimax(board,depth-1,false);
					
					if(result>maxscore){

						maxscore=result;
						
						if(depth==8){
							ai=i;
							file<<"maxscore,ai"<<maxscore<<" "<<ai<<endl;
						}
					}
					board[i][j]=0;
					break;
				}
				
			}
			if(maxscore==100) break;
			
		}
		file<<"Maxscore,depth,ai: "<<maxscore<<" "<<depth<<" "<<ai<<endl;
		return maxscore;
	}else{
		int minscore=1000;
		for(int i=0;i<7;i++){
			for(int j=0;j<7;j++){
				if(board[i][j]==0){
					board[i][j]=1;
					int result=minimax(board,depth-1,true);
					if(result<minscore){
						minscore=result;
					}
					board[i][j]=0;
					break;
				}
			}
			if(minscore==-200) break;

		}
		return minscore;
	}
}

void AI::check4X4(int board[::x][::y],int x,int y){
	score[0]=0;score[1]=0;
	for(int i=0;i<4;i++){
		if(board[x+i][y+0]==1&&board[x+i][y+1]==1&&board[x+i][y+2]==1&&board[x+i][y+3]==1){
			score[0]++;
		}
	}
	for(int i=0;i<4;i++){
			if(board[x+0][y+i]==1&&board[x+1][y+i]==1&&board[x+2][y+i]==1&&board[x+3][y+i]==1){
				score[0]++;
			}
	}
	if(board[x+0][y+0]==1&&board[x+1][y+1]==1&&board[x+2][y+2]==1&&board[x+3][y+3]==1){
		score[0]++;
	}
	if(board[x+0][y+3]==1&&board[x+1][y+2]==1&&board[x+2][y+1]==1&&board[x+3][y+0]==1){
		score[0]++;
	}

	for(int i=0;i<4;i++){
		if(board[x+i][y+0]==2&&board[x+i][y+1]==2&&board[x+i][y+2]==2&&board[x+i][y+3]==2){
			score[1]++;
		}
	}
	for(int i=0;i<4;i++){
			if(board[x+0][y+i]==2&&board[x+1][y+i]==2&&board[x+2][y+i]==2&&board[x+3][y+i]==2){
				score[1]++;
			}
	}
	if(board[x+0][y+0]==2&&board[x+1][y+1]==2&&board[x+2][y+2]==2&&board[x+3][y+3]==2){
		score[1]++;
	}
	if(board[x+0][y+3]==2&&board[x+1][y+2]==2&&board[x+2][y+1]==2&&board[x+3][y+0]==2){
		score[1]++;
	}

	return;
}

void AI::check3X3(int board[::x][::y],int x,int y){
	score[0]=0;score[1]=0;
	for(int i=0;i<3;i++){
			if(board[x+i][y+0]==1&&board[x+i][y+1]==1&&board[x+i][y+2]==1){
				score[0]++;
			}
	}
	for(int i=0;i<3;i++){
			if(board[x+0][y+i]==1&&board[x+1][y+i]==1&&board[x+2][y+i]==1){
				score[0]++;
			}
	}
	if(board[x+0][y+0]==1&&board[x+1][y+1]==1&&board[x+2][y+2]==1){
		score[0]++;
	}
	if(board[x+0][y+2]==1&&board[x+1][y+1]==1&&board[x+2][y+0]==1){
		score[0]++;
	}

	for(int i=0;i<3;i++){
			if(board[x+i][y+0]==2&&board[x+i][y+1]==2&&board[x+i][y+2]==2){
				score[1]++;
			}
	}
	for(int i=0;i<3;i++){
			if(board[x+0][y+i]==2&&board[x+1][y+i]==2&&board[x+2][y+i]==2){
				score[1]++;
			}
	}
	if(board[x+0][y+0]==1&&board[x+1][y+1]==1&&board[x+2][y+2]==1){
		score[1]++;
	}
	if(board[x+0][y+2]==2&&board[x+1][y+1]==2&&board[x+2][y+0]==2){
		score[1]++;
	}
}

int AI::checkTotalScore(int board[::x][::y]){
	int totalscore=0;
	
		for(int p=0;p<=4;p++){
			for(int q=0;q<=4;q++){
				check3X3(board,p,q);
				totalscore+=-5*score[0];
				totalscore+=4*score[1];
			}
		}
	
	return totalscore;
}

bool AI::checkdraw(int board[::x][::y]){
	bool drw=true;
	for(int i=0;i<7;i++){
		if(board[i][6]==0){
			drw=false;
			break;
		}
	}
	if(drw){
		return true;
	}
	return false;
}


void GAME::assign_columns(){
	Button* columns[::x];
	for(int i=0;i<(::x);i++){
		columns[i]=new Button(i,::x,::y,::w,::h);
	}
}

GAME::GAME(bool c){
	if(!c){
		throw -1;
	}

	//initializing elements of data to zero
	for(int i=0;i<::x;i++){
		for(int j=0;j<::y;j++){
			data[i][j]=0;
		}
	}
	//initializations of some dimensions of the board
	outerRectangle=new Shape(::x,::y,::w,::h);

	//calling function to create buttons for columns of the board
	assign_columns();

}

void Button::click_made(){
	for(int j=0;j<::y;j++){
		if(data[i][j]==0){
			data[i][j]=1;
			fl_color(fl_rgb_color(200,0,0));
			fl_pie (topLeftx+i*xspacing+20,topLefty+(y-j-1)*yspacing+5,yspacing-5,yspacing-5,0,360);
			file<<"------------------------\n";
			AImove(data);
			file<<"AImove: "<<ai<<endl;
			for(int k=0;k<y;k++){
				if(data[ai][k]==0){
					data[ai][k]=2;
					fl_color(fl_rgb_color(0,0,200));
					fl_pie (topLeftx+ai*xspacing+20,topLefty+(::y-k-1)*yspacing+5,yspacing-5,yspacing-5,0,360);
					break;
				}
			}
			break;
		
		}
	}
	int tempWin=winner(data);
	if(score[0]!=0){
		std::cout<<"Player1 Wins\n";
		fl_color(fl_rgb_color(200,0,0));
		fl_draw("Player1 Wins",500-60,50);
	}else if(score[1]!=0){
		std::cout<<"Player2 Wins\n";
		fl_color(fl_rgb_color(0,0,200));
		fl_draw("Player2 Wins",500-60,50);
	}else if(checkdraw(data)){
		std::cout<<"Draw\n";
		fl_color(fl_rgb_color(0,0,0));
		fl_draw("Draw",500-40,50);
	}
	file<<"Moves"<<::moves<<endl;
}

int Button::handle(int e){

	if(score[0]!=0||score[1]!=0||checkdraw(data)){
		Fl::wait(0.5);
		sleep(5);
		exit(0);
	}
	if (e==FL_PUSH) {
		moves++;
		click_made();
	}

	return 1;

}

int main(int argc, char *argv[]){
	Fl_Window *window;
	window = new Fl_Window (w,h,"Plot4");
	window->color(FL_WHITE);
	fl_font(1,30);
	/*file.open("gameBugData.txt",ios::out);
	if(!file){
		cout<<"Bad Luck"<<endl;
		return 0;
	}*/
	try{
		GAME sample(true);
	}catch(int c){
		if(c==-1){
			std::cout<<"Inappropriate instantiation of object of class GAME";
		}
	}

    window->end();
    window->show();
    return(Fl::run());

}
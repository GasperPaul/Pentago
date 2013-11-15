#ifndef UTILS_H
#define UTILS_H

/*
 * Helper class for "K in a row" on N x M board.
 * Author: Youw.
 * Version: 1.25 beta.
 */
class Utils {
private:

	template <typename T>
	inline static T Sgn(T val) {
		return (T(0) < val) - (val < T(0));
	};

	/*
	 * Competitive line checking.
	 * Class T must implement operator [].
	 */
	template <typename T>
	inline static int LittleCheck(T A, int cols, int rows, int col, int row, int dCol, int dRow, int len) {
		/*if (len) {
			if (sgn(A[col][row]) == SGN)
				return LittleCheck(A, col + dCol, row + dRow, dCol, dRow, len - 1, SGN);
			else
				return 0;
		} else return SGN;*/
		int sgn = 0;
		int sgnCount = 0;
		do { 
			int tmpSgn=Sgn(A[col][row]);
			if ( tmpSgn == sgn) {
				if(sgn && ++sgnCount == len) {
					return sgn;
				}
			} else {
				sgn=tmpSgn;
				sgnCount=1;
			}
			col += dCol;
			row += dRow;
		} while (((unsigned int)col<(unsigned int)cols)&&((unsigned int)row<(unsigned int)rows));
		return 0;
	}

public:

	/*
	 * Win condition checking function.
	 * Class T must implement operator [].
	 */
	template <typename T>
	static int Check(T A, int cols, int rows, int len) {
		if ((len>cols)||(len>rows)) throw "Fuck int...";
		
		//прохід по всіх рядках
		for (int j=0; j<rows; j++) {
			if (int res=LittleCheck(A,cols,rows,0,j,1,0,len)) {
				return res;
			}
		}
		
		//прохід по всіх стовбцях
		for (int i=0; i<cols; i++) {
			if (int res=LittleCheck(A,cols,rows,i,0,0,1,len)) {
				return res;
			}
		}

		//прохід по всіх головних діагоналях
	
		//нижня піддіагональ
		for(int j=rows-len; j>=0; j--){
			if (int res=LittleCheck(A,cols,rows,0,j,1,1,len)) {
				return res;
			}
		}
		//верхня наддіагональ
		for(int i=1; i<=cols-len; i++){
			if (int res=LittleCheck(A,cols,rows,i,0,1,1,len)) {
				return res;
			}
		}
	
		//прохід по всіх побічних діагоналях
	
		//верхня наддіагональ
		for(int i=len-1; i<cols; i++){
			if (int res=LittleCheck(A,cols,rows,i,0,-1,1,len)) {
				return res;
			}
		}
		//нижня піддіагональ
		for(int j=1; j<=rows-len; j++){
			if (int res=LittleCheck(A,cols,rows,cols-1,j,-1,1,len)) {
				return res;
			}
		}
		return 0;
	}
};

#endif // UTILS_H

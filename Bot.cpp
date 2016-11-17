#include <iostream>
#include <stdio.h>
#include <utility>
#include <algorithm>
#include <fstream>

using namespace std;

#define LEARNING 0.0005

struct weights
{
	double g1 , g2 , g3;
};

//Coefficients to be Calculated
double gamma1 = 7.0;
double gamma_mobility = 1.0;
double gamma_chunk = 3.0;
int gamma_turns = 10;
int gamma_update = 0;
weights weightBackup[49][2];
double prevVal = 0;


void readBackup6(int N)
{
	ifstream f("backup6.txt");

	for (int i = 0; i < N; ++i)
	{
		f >> weightBackup[i][0].g1 >> weightBackup[i][0].g2 >> weightBackup[i][0].g3;
		f >> weightBackup[i][1].g1 >> weightBackup[i][1].g2 >> weightBackup[i][1].g3;
	}
}

void writeBackup(int N)
{
	ofstream f("backup.txt");

	for (int i = 0; i < N; ++i)
	{
		f << weightBackup[i][0].g1 <<" "<< weightBackup[i][0].g2 <<" "<< weightBackup[i][0].g3<<endl;
		f << weightBackup[i][1].g1 << " "<<  weightBackup[i][1].g2 <<" "<< weightBackup[i][1].g3<<endl;
	}
}

void changeWeights(int flag , int n , double learningRate , double difference , weights featureVals )
{
	weightBackup[n][flag].g1 +=  learningRate*difference*featureVals.g1;
	weightBackup[n][flag].g2 +=  learningRate*difference*featureVals.g2;
	weightBackup[n][flag].g3 +=  learningRate*difference*featureVals.g3;

}

class Bot6
{

public:
	Bot6(string pt,int n,double a,double b,double c,int d,int e)
	{

		MAXDEPTH = 3;

		MAXDEPTH_BY_2 = MAXDEPTH/2;

		N = n;
		board[7][7];
		counter[26];
		total = 25;
		NoOfTurns = 25;
		position = make_pair(0,0);
		fromForOrder = make_pair(0,0);

		positions[0] = make_pair(0,0);
		positions[1] = make_pair(0,0);
		positions[2] = make_pair(0,0);
		fromForOrders[0] = make_pair(0,0);
		fromForOrders[1] = make_pair(0,0);
		fromForOrders[2] = make_pair(0,0);
		countForPositions = -1;


		gamma1 = a;
		gamma_mobility = b;
		gamma_chunk = c;
		gamma_turns = d;
		gamma_update = e;

		final_value = 0;


		playerType = pt;
		N = n;
		total = N*N;
		NoOfTurns = total;
		
		for( int i = 0 ; i < N ; i++)
		{
			for(int j = 0 ; j < N ; j++)
			{
				board[i][j] = '$';
				counter[j] = N;
			}
		}

	}

	int MAXDEPTH;

	int MAXDEPTH_BY_2;

	string playerType;

	int N;
	char board[7][7];
	int counter[26];
	int total;
	int NoOfTurns;
	pair<int,int> position;
	pair<int,int> fromForOrder;

	pair<int,int> positions[3];
	pair<int,int> fromForOrders[3];
	int countForPositions;


	//Coefficients to be Calculated
	double gamma1;
	double gamma_mobility;
	double gamma_chunk;
	int gamma_turns;
	int gamma_update;

	double final_value;
	double rowReward[7];
	double columnReward[7];
	double mobilityRowValue[7];
	double mobilityColumnValue[7];
	double chunkRowReward[7];
	double chunkColumnReward[7];

	// double maxval( double alpha , double beta , int depth);
	// double minval( double alpha , double beta , char colour , int depth);
	// double maxvalSecondary( double alpha , double beta , int depth);
	// double minvalSecondary( double alpha , double beta , char colour , int depth);


	double rowValue(int rowIndex)
	{
		int ret = 0;
		for(int i = 0;i < N ;i++)
		{
			if(board[rowIndex][i] == '$')
			{
				int j = 1;
				int node_value = 0;
				while(i-j > -1 && i+j < N)
				{
					if(board[rowIndex][i-j] == board[rowIndex][i+j] && board[rowIndex][i-j] != '$' && board[rowIndex][i+j] != '$')
					{
						node_value += 2*j+1;
						j++;
					}
					else
					{
						break;
					}
				}
				ret += node_value;
			}
			else
			{
				int j = 1;
				int node_value = 0;
				while(i-j > -1 && i+j < N)
				{
					if(board[rowIndex][i-j] == board[rowIndex][i+j] && board[rowIndex][i-j] != '$' && board[rowIndex][i+j] != '$')
					{
						node_value += 2*j+1;
						j++;
					}
					else
					{
						break;
					}
				}
				if(i+1 < N)
				{
					if(board[rowIndex][i+1] == board[rowIndex][i])
					{
						node_value += 2;
						int k = 1;
						while(i-k > -1 && i+k+1 < N)
						{
							if(board[rowIndex][i-k] == board[rowIndex][i+k+1] && board[rowIndex][i-k] != '$' && board[rowIndex][i+k+1] != '$')
							{
								node_value += 2*k+2;
								k++;
							}
							else
							{
								break;
							}
						}
					}
				}
				ret += node_value;
			}
		}
		return (double)ret;
	}



	double columnValue(int columnIndex)
	{
		int ret = 0;
		for(int i = 0;i < N ;i++)
		{
			if(board[i][columnIndex] == '$')
			{
				int j = 1;
				int node_value = 0;
				while(i-j > -1 && i+j < N)
				{
					if(board[i-j][columnIndex] == board[i+j][columnIndex] && board[i-j][columnIndex] != '$' && board[i+j][columnIndex] != '$')
					{
						node_value += 2*j+1;
						j++;
					}
					else
					{
						break;
					}
				}
				ret += node_value;
			}
			else
			{
				int j = 1;
				int node_value = 0;
				while(i-j > -1 && i+j < N)
				{
					if(board[i-j][columnIndex] == board[i+j][columnIndex] && board[i-j][columnIndex] != '$' && board[i+j][columnIndex] != '$')
					{
						node_value += 2*j+1;
						j++;
					}
					else
					{
						break;
					}
				}
				if(i+1 < N)
				{
					if(board[i+1][columnIndex] == board[i][columnIndex])
					{
						node_value += 2;
						int k = 1;
						while(i-k > -1 && i+k+1 < N)
						{
							if(board[i-k][columnIndex] == board[i+k+1][columnIndex] && board[i-k][columnIndex] != '$' && board[i+k+1][columnIndex] != '$')
							{
								node_value += 2*k+2;
								k++;
							}
							else
							{
								break;
							}
						}
					}
				}
				ret += node_value;
			}
		}
		return (double)ret;
	}


	double mobilityRow(int i)
	{
		int ret = 0;
		
		int last_occupied = -1;
		int j = 0;
		bool flag = false;
		while(j < N)
		{
			if(board[i][j] !=  '$')
			{
				if(last_occupied == -1)
				{
					ret += (j - last_occupied - 1);
				}
				else
				{
					ret += 2 * (j - last_occupied - 1);
				}
				last_occupied = j;
				flag = true;
			}
			
			j++;
		}
		if(flag)
		{
			ret += j - last_occupied - 1;
		}
		
		return (double)ret;
	}


	double mobilityColumn(int i)
	{
		int ret = 0;
		
		int last_occupied = -1;
		int j = 0;
		bool flag = false;
		while(j < N)
		{
			if(board[j][i] !=  '$')
			{
				if(last_occupied == -1)
				{
					ret += (j - last_occupied - 1);
				}
				else
				{
					ret += 2 * (j - last_occupied - 1);
				}
				last_occupied = j;
				flag = true;
				
			}
			j++;
		}
		if(flag)
		{
			ret += j - last_occupied - 1;
		}

		return (double)ret;
	}


	double chunkRow(int rowIndex)
	{
		int ret = 0;
		int j = 0;
		int size = 0;
		char last = '$';
		while(j < N)
		{
			if(board[rowIndex][j] != '$' && board[rowIndex][j] == last)
			{
				size++;
			}
			else
			{
				ret += size*size;
				size = 0;
			}
			last = board[rowIndex][j];
			j++;
		}
		return (double)(ret+size*size);
	}


	double chunkColumn(int columnIndex)
	{
		int ret = 0;
		int j = 0;
		int size = 0;
		char last = '$';
		while(j < N)
		{
			if(board[j][columnIndex] != '$' && board[j][columnIndex] == last)
			{
				size++;
			}
			else
			{
				ret += size*size;
				size = 0;
			}
			last = board[j][columnIndex];
			j++;
		}
		return (double)(ret+size*size);
	}


	double ObjectiveFunctionValue()
	{
		double ret = 0;
		for(int i = 0;i < N;i++)
		{
			// ret += gamma1 * (rowReward[i] + columnReward[i]) + gamma_mobility * (mobilityRowValue[i] + mobilityColumnValue[i]) + 
			// 			gamma_chunk * (chunkRowReward[i] + chunkColumnReward[i]);
			ret += gamma1 * (rowValue(i) + columnValue(i)) + gamma_mobility * (mobilityRow(i) + mobilityColumn(i)) + 
					gamma_chunk * (chunkRow(i) + chunkColumn(i));
		}
		return ret;
	}


	double ObjectiveFunctionInitialisation()
	{
		double ret = 0;
		for(int i = 0;i < N;i++)
		{
			rowReward[i] = rowValue(i);
			columnReward[i] = columnValue(i);
			mobilityRowValue[i] = mobilityRow(i);
			mobilityColumnValue[i] = mobilityColumn(i);
			chunkRowReward[i] = chunkRow(i);
			chunkColumnReward[i] = chunkColumn(i);
			ret += gamma1 * (rowReward[i] + columnReward[i]) + gamma_mobility * (mobilityRowValue[i] + mobilityColumnValue[i]) + 
						gamma_chunk * (chunkRowReward[i] + chunkColumnReward[i]);
		}
		return ret;
	}

	double getFeature1()
	{
		double ret = 0;
		for(int i = 0;i < N;i++)
		{
			rowReward[i] = rowValue(i);
			columnReward[i] = columnValue(i);
			ret += (rowReward[i] + columnReward[i]) ;
		}
		return ret;

	}

	double getFeature2()
	{
		double ret = 0;
		for(int i = 0;i < N;i++)
		{
			mobilityRowValue[i] = mobilityRow(i);
			mobilityColumnValue[i] = mobilityColumn(i);
			ret +=  (mobilityRowValue[i] + mobilityColumnValue[i]);
		}
		return ret;
		
	}

	double getFeature3()
	{
		double ret = 0;
		for(int i = 0;i < N;i++)
		{
			chunkRowReward[i] = chunkRow(i);
			chunkColumnReward[i] = chunkColumn(i);
			ret += (chunkRowReward[i] + chunkColumnReward[i]);
		}
		return ret;
		
	}

	void printBoard()
	{
		for( int i = 0 ; i < N ; i++)
		{
			for(int j = 0 ; j < N ; j++)
			{
				cerr << board[i][j] << "  ";
			}
			cerr << "\n";
		}
		cerr << "\n";
	}

	void getValidMoves( pair<int,int> pos , pair<int,int> &rangeX , pair<int,int> &rangeY )
	{
		rangeX.first = 0;
		rangeX.second = N-1;

		rangeY.first = 0;
		rangeY.second = N-1;

		for (int i = pos.first+1 ; i < N ; ++i)
		{
			if( board[i][pos.second] != '$' )
				{
					rangeX.second = i-1;
					break;
				}
		}

		for (int i = pos.first-1 ; i >= 0 ; i--)
		{
			if( board[i][pos.second] != '$' )
				{
					rangeX.first = i+1;
					break;
				}
		}

		for (int i = pos.second+1 ; i < N ; ++i)
		{
			if( board[pos.first][i] != '$' )
				{
					rangeY.second = i-1;
					break;
				}
		}

		for (int i = pos.second-1 ; i >= 0 ; i--)
		{
			if( board[pos.first][i] != '$' )
				{
					rangeY.first = i+1;
					break;
				}
		}
	}



	double minval(double alpha , double beta , char colour , int depth)
	{
		if( depth == min(MAXDEPTH,NoOfTurns))
		{
			return final_value;//ObjectiveFunctionValue();
		}
		
		
		double child ;

		
		for( int i = 0 ; i < N ; i++)
		{
			for(int j = 0 ; j < N ; j++)
			{
				double prev_final_value = final_value;
				if( board[i][j] == '$')
				{

					double prevRowReward = rowReward[i];
					double prevColumnReward = columnReward[j];
					double prevMobilityRowValue = mobilityRowValue[i];
					double prevMobilityColumnValue = mobilityColumnValue[j];
					double prevChunkRowValue = chunkRowReward[i];
					double prevChunkColumnValue = chunkColumnReward[j];

					board[i][j] = colour+'A';
					counter[colour]-- ;
					total--;
					

					rowReward[i] = rowValue(i);
					columnReward[j] = columnValue(j);
					mobilityRowValue[i] = mobilityRow(i);
					mobilityColumnValue[j] = mobilityColumn(j);
					chunkRowReward[i] = chunkRow(i);
					chunkColumnReward[j] = chunkColumn(j);

					final_value += gamma1 * ( rowReward[i] + columnReward[j] - prevRowReward - prevColumnReward ) + 
									gamma_mobility * (mobilityRowValue[i] + mobilityColumnValue[j] - prevMobilityRowValue - prevMobilityColumnValue) + 
									gamma_chunk * (chunkRowReward[i] + chunkColumnReward[j] - prevChunkRowValue - prevChunkColumnValue);

					child = maxval( alpha , beta , depth+1 );

					if( child < beta )
					{
						beta = child;
						
						if( depth == 0 )
						{
							if( MAXDEPTH_BY_2 < NoOfTurns )
							{
								positions[2].first = positions[1].first;
								positions[1].first = positions[0].first;
								positions[0].first = position.first;
								positions[2].second = positions[1].second;
								positions[1].second = positions[0].second;
								positions[0].second = position.second;
								countForPositions++;
							}
							position.first = i;
							position.second = j;
						}
					}

					
					board[i][j] = '$';
					counter[colour]++ ;
					total++;

					rowReward[i] = prevRowReward;
					columnReward[j] = prevColumnReward;
					mobilityRowValue[i] = prevMobilityRowValue;
					mobilityColumnValue[j] = prevMobilityColumnValue;
					chunkRowReward[i] = prevChunkRowValue;
					chunkColumnReward[j] = prevChunkColumnValue;

					final_value = prev_final_value;
					
					if( alpha >= beta )
						{
							return child;
						}
				}
			}	
		}

		return beta;

	}



	double maxval( double alpha , double beta , int depth)
	{
		if( depth == min(MAXDEPTH,NoOfTurns))
		{
			return final_value;//ObjectiveFunctionValue();
		}
		
		
		pair<int,int> rangeX , rangeY ;
		double child ;
		char colour;


		for( int i = 0 ; i < N ; i++)
		{
			for(int j = 0 ; j < N ; j++)
			{
				if( board[i][j] != '$' )
				{
					getValidMoves( make_pair(i,j) , rangeX , rangeY );
					
					double prevRowReward = rowReward[i];
					double prevColumnReward = columnReward[j];
					double prevMobilityRowValue = mobilityRowValue[i];
					double prevMobilityColumnValue = mobilityColumnValue[j];
					double prevChunkRowValue = chunkRowReward[i];
					double prevChunkColumnValue = chunkColumnReward[j];
					final_value -= (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
										gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
					double prev_final_value = final_value;

					colour = board[i][j];
					
					board[i][j] = '$';


					for (int r = rangeX.first ; r < rangeX.second+1 ; ++r)
					{
						if(r != i)
						{

							double prevRowRewardR = rowReward[r];
							double prevMobilityRowValueR = mobilityRowValue[r];
							double prevChunkRowValueR = chunkRowReward[r];

							board[r][j] = colour;
							child = 0;

							rowReward[i] = rowValue(i);
							rowReward[r] = rowValue(r);
							columnReward[j] = columnValue(j);
							mobilityRowValue[i] = mobilityRow(i);
							mobilityRowValue[r] = mobilityRow(r);
							mobilityColumnValue[j] = mobilityColumn(j);
							chunkRowReward[i] = chunkRow(i);
							chunkRowReward[r] = chunkRow(r);
							chunkColumnReward[j] = chunkColumn(j);

							final_value += gamma1 * (rowReward[i] + rowReward[r] + columnReward[j] - prevRowRewardR) + 
										gamma_mobility * (mobilityRowValue[i] + mobilityRowValue[r] + mobilityColumnValue[j] - prevMobilityRowValueR) + 
										gamma_chunk * (chunkRowReward[i] + chunkRowReward[r] + chunkColumnReward[j] - prevChunkRowValueR);


							for( int c = 0 ; c < N ; c++)
							{
								if(counter[c] > 0)
								{
									child += (double)(counter[c]) / (double)total * minval( alpha , beta , (char)c , depth+1 ) ;
								}
							}

							if( child > alpha )
							{
								alpha = child;
								
								if( depth == 0 )
								{
									if( MAXDEPTH_BY_2 < NoOfTurns )
									{
										positions[2].first = positions[1].first;
										positions[1].first = positions[0].first;
										positions[0].first = position.first;
										positions[2].second = positions[1].second;
										positions[1].second = positions[0].second;
										positions[0].second = position.second;
										fromForOrders[2].first = fromForOrders[1].first;
										fromForOrders[1].first = fromForOrders[0].first;
										fromForOrders[0].first = fromForOrder.first;
										fromForOrders[2].second = fromForOrders[1].second;
										fromForOrders[1].second = fromForOrders[0].second;
										fromForOrders[0].second = fromForOrder.second;

										countForPositions++;
									}
									position.first = r;
									position.second = j;
									fromForOrder.first = i;
									fromForOrder.second = j;
								}
							}

							board[r][j] = '$';
							
							rowReward[r] = prevRowRewardR;
							mobilityRowValue[r] = prevMobilityRowValueR;
							chunkRowReward[r] = prevChunkRowValueR;
							final_value = prev_final_value;
							
							if( alpha >= beta )
							{
								rowReward[i] = prevRowReward;
								columnReward[j] = prevColumnReward;
								mobilityRowValue[i] = prevMobilityRowValue;
								mobilityColumnValue[j] = prevMobilityColumnValue;
								chunkRowReward[i] = prevChunkRowValue;
								chunkColumnReward[j] = prevChunkColumnValue;
								final_value += (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
													gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
								board[i][j] = colour;
								return child;
							}
						}
					}

					
					for (int r = rangeY.first ; r < rangeY.second+1 ; ++r)
					{
						if(r != j)
						{

							double prevColumnRewardR = columnReward[r];
							double prevMobilityColumnValueR = mobilityColumnValue[r];
							double prevChunkColumnValueR = chunkColumnReward[r];
							
							board[i][r] = colour;
							child = 0;

							rowReward[i] = rowValue(i);
							columnReward[r] = columnValue(r);
							columnReward[j] = columnValue(j);
							mobilityRowValue[i] = mobilityRow(i);
							mobilityColumnValue[r] = mobilityColumn(r);
							mobilityColumnValue[j] = mobilityColumn(j);
							chunkRowReward[i] = chunkRow(i);
							chunkColumnReward[r] = chunkColumn(r);
							chunkColumnReward[j] = chunkColumn(j);


							final_value += gamma1 * (rowReward[i] + columnReward[r] + columnReward[j] - prevColumnRewardR) + 
										gamma_mobility * (mobilityRowValue[i] + mobilityColumnValue[r] + mobilityColumnValue[j] - prevMobilityColumnValueR) + 
										gamma_chunk * (chunkRowReward[i] + chunkColumnReward[r] + chunkColumnReward[j] - prevChunkColumnValueR);
							
							for( int c = 0 ; c < N ; c++)
							{
								if(counter[c] > 0)
								{
									child += (double)(counter[c]) / (double)total * minval( alpha , beta , (char)c , depth+1 ) ;
								}
							}

							if( child > alpha )
							{
								alpha = child;
								
								if( depth == 0 )
								{
									if( MAXDEPTH_BY_2 < NoOfTurns )
									{
										positions[2].first = positions[1].first;
										positions[1].first = positions[0].first;
										positions[0].first = position.first;
										positions[2].second = positions[1].second;
										positions[1].second = positions[0].second;
										positions[0].second = position.second;
										fromForOrders[2].first = fromForOrders[1].first;
										fromForOrders[1].first = fromForOrders[0].first;
										fromForOrders[0].first = fromForOrder.first;
										fromForOrders[2].second = fromForOrders[1].second;
										fromForOrders[1].second = fromForOrders[0].second;
										fromForOrders[0].second = fromForOrder.second;

										countForPositions++;
									}
									position.first = i;
									position.second = r;
									fromForOrder.first = i;
									fromForOrder.second = j;
								}
							}

							board[i][r] = '$';
							
							columnReward[r] = prevColumnRewardR;
							mobilityColumnValue[r] = prevMobilityColumnValueR;
							chunkColumnReward[r] = prevChunkColumnValueR;
							final_value = prev_final_value;
							
							
							if( alpha >= beta )
							{
								rowReward[i] = prevRowReward;
								columnReward[j] = prevColumnReward;
								mobilityRowValue[i] = prevMobilityRowValue;
								mobilityColumnValue[j] = prevMobilityColumnValue;
								chunkRowReward[i] = prevChunkRowValue;
								chunkColumnReward[j] = prevChunkColumnValue;
								final_value += (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
													gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
								board[i][j] = colour;
								return child;
							}
						
						}
					}

					rowReward[i] = prevRowReward;
					columnReward[j] = prevColumnReward;
					mobilityRowValue[i] = prevMobilityRowValue;
					mobilityColumnValue[j] = prevMobilityColumnValue;
					chunkRowReward[i] = prevChunkRowValue;
					chunkColumnReward[j] = prevChunkColumnValue;
					final_value += (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
										gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
					board[i][j] = colour;

				}

			}
		}

		return alpha;
	}

	double minvalSecondary(double alpha , double beta , char colour , int depth)
	{
		if( depth == min(MAXDEPTH,NoOfTurns))
		{
			return (double)final_value;
		}
		
		
		double child ;

		
		for( int i = 0 ; i < N ; i++)
		{
			for(int j = 0 ; j < N ; j++)
			{
				double prev_final_value = final_value;
				if( board[i][j] == '$')
				{

					double prevRowReward = rowReward[i];
					double prevColumnReward = columnReward[j];
					double prevMobilityRowValue = mobilityRowValue[i];
					double prevMobilityColumnValue = mobilityColumnValue[j];
					double prevChunkRowValue = chunkRowReward[i];
					double prevChunkColumnValue = chunkColumnReward[j];

					board[i][j] = colour+'A';
					counter[colour]-- ;
					total--;
					

					rowReward[i] = rowValue(i);
					columnReward[j] = columnValue(j);
					mobilityRowValue[i] = mobilityRow(i);
					mobilityColumnValue[j] = mobilityColumn(j);
					chunkRowReward[i] = chunkRow(i);
					chunkColumnReward[j] = chunkColumn(j);

					final_value += gamma1 * ( rowReward[i] + columnReward[j] - prevRowReward - prevColumnReward ) + 
									gamma_mobility * (mobilityRowValue[i] + mobilityColumnValue[j] - prevMobilityRowValue - prevMobilityColumnValue) + 
									gamma_chunk * (chunkRowReward[i] + chunkColumnReward[j] - prevChunkRowValue - prevChunkColumnValue);

					child = maxvalSecondary( alpha , beta , depth+1 );
					if( child < beta )
					{
						beta = child;
					}

					
					board[i][j] = '$';
					counter[colour]++ ;
					total++;

					rowReward[i] = prevRowReward;
					columnReward[j] = prevColumnReward;
					mobilityRowValue[i] = prevMobilityRowValue;
					mobilityColumnValue[j] = prevMobilityColumnValue;
					chunkRowReward[i] = prevChunkRowValue;
					chunkColumnReward[j] = prevChunkColumnValue;

					final_value = prev_final_value;
					
					if( alpha >= beta )
						{
							return child;
						}
				}
			}	
		}

		return beta;

	}



	double maxvalSecondary( double alpha , double beta , int depth)
	{
		if( depth == min(MAXDEPTH,NoOfTurns))
		{
			return (double)final_value;
		}
		
		
		pair<int,int> rangeX , rangeY ;
		double child ;
		char colour;


		for( int i = 0 ; i < N ; i++)
		{
			for(int j = 0 ; j < N ; j++)
			{
				if( board[i][j] != '$' )
				{
					getValidMoves( make_pair(i,j) , rangeX , rangeY );
					
					double prevRowReward = rowReward[i];
					double prevColumnReward = columnReward[j];
					double prevMobilityRowValue = mobilityRowValue[i];
					double prevMobilityColumnValue = mobilityColumnValue[j];
					double prevChunkRowValue = chunkRowReward[i];
					double prevChunkColumnValue = chunkColumnReward[j];
					final_value -= (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
										gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
					double prev_final_value = final_value;

					colour = board[i][j];
					
					board[i][j] = '$';


					for (int r = rangeX.first ; r < rangeX.second+1 ; ++r)
					{
						if(r != i)
						{

							double prevRowRewardR = rowReward[r];
							double prevMobilityRowValueR = mobilityRowValue[r];
							double prevChunkRowValueR = chunkRowReward[r];

							board[r][j] = colour;
							child = 0;

							rowReward[i] = rowValue(i);
							rowReward[r] = rowValue(r);
							columnReward[j] = columnValue(j);
							mobilityRowValue[i] = mobilityRow(i);
							mobilityRowValue[r] = mobilityRow(r);
							mobilityColumnValue[j] = mobilityColumn(j);
							chunkRowReward[i] = chunkRow(i);
							chunkRowReward[r] = chunkRow(r);
							chunkColumnReward[j] = chunkColumn(j);

							final_value += gamma1 * (rowReward[i] + rowReward[r] + columnReward[j] - prevRowRewardR) + 
										gamma_mobility * (mobilityRowValue[i] + mobilityRowValue[r] + mobilityColumnValue[j] - prevMobilityRowValueR) + 
										gamma_chunk * (chunkRowReward[i] + chunkRowReward[r] + chunkColumnReward[j] - prevChunkRowValueR);


							for( int c = 0 ; c < N ; c++)
							{
								if(counter[c] > 0)
								{
									child += (double)(counter[c]) / (double)total * minvalSecondary( alpha , beta , (char)c , depth+1 ) ;
								}
							}

							if( child > alpha )
							{
								alpha = child;
							}

							board[r][j] = '$';
							
							rowReward[r] = prevRowRewardR;
							mobilityRowValue[r] = prevMobilityRowValueR;
							chunkRowReward[r] = prevChunkRowValueR;
							final_value = prev_final_value;
							
							if( alpha >= beta )
							{
								rowReward[i] = prevRowReward;
								columnReward[j] = prevColumnReward;
								mobilityRowValue[i] = prevMobilityRowValue;
								mobilityColumnValue[j] = prevMobilityColumnValue;
								chunkRowReward[i] = prevChunkRowValue;
								chunkColumnReward[j] = prevChunkColumnValue;
								final_value += (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
													gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
								board[i][j] = colour;
								return child;
							}
						}
					}

					
					for (int r = rangeY.first ; r < rangeY.second+1 ; ++r)
					{
						if(r != j)
						{

							double prevColumnRewardR = columnReward[r];
							double prevMobilityColumnValueR = mobilityColumnValue[r];
							double prevChunkColumnValueR = chunkColumnReward[r];
							
							board[i][r] = colour;
							child = 0;

							rowReward[i] = rowValue(i);
							columnReward[r] = columnValue(r);
							columnReward[j] = columnValue(j);
							mobilityRowValue[i] = mobilityRow(i);
							mobilityColumnValue[r] = mobilityColumn(r);
							mobilityColumnValue[j] = mobilityColumn(j);
							chunkRowReward[i] = chunkRow(i);
							chunkColumnReward[r] = chunkColumn(r);
							chunkColumnReward[j] = chunkColumn(j);


							final_value += gamma1 * (rowReward[i] + columnReward[r] + columnReward[j] - prevColumnRewardR) + 
										gamma_mobility * (mobilityRowValue[i] + mobilityColumnValue[r] + mobilityColumnValue[j] - prevMobilityColumnValueR) + 
										gamma_chunk * (chunkRowReward[i] + chunkColumnReward[r] + chunkColumnReward[j] - prevChunkColumnValueR);
							
							for( int c = 0 ; c < N ; c++)
							{
								if(counter[c] > 0)
								{
									child += (double)(counter[c]) / (double)total * minvalSecondary( alpha , beta , (char)c , depth+1 ) ;
								}
							}

							if( child > alpha )
							{
								alpha = child;
							}

							board[i][r] = '$';
							
							columnReward[r] = prevColumnRewardR;
							mobilityColumnValue[r] = prevMobilityColumnValueR;
							chunkColumnReward[r] = prevChunkColumnValueR;
							final_value = prev_final_value;
							
							
							if( alpha >= beta )
							{
								rowReward[i] = prevRowReward;
								columnReward[j] = prevColumnReward;
								mobilityRowValue[i] = prevMobilityRowValue;
								mobilityColumnValue[j] = prevMobilityColumnValue;
								chunkRowReward[i] = prevChunkRowValue;
								chunkColumnReward[j] = prevChunkColumnValue;
								final_value += (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
													gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
								board[i][j] = colour;
								return child;
							}
						
						}
					}

					rowReward[i] = prevRowReward;
					columnReward[j] = prevColumnReward;
					mobilityRowValue[i] = prevMobilityRowValue;
					mobilityColumnValue[j] = prevMobilityColumnValue;
					chunkRowReward[i] = prevChunkRowValue;
					chunkColumnReward[j] = prevChunkColumnValue;
					final_value += (gamma1 * (prevRowReward + prevColumnReward) + gamma_mobility * (prevMobilityRowValue + prevMobilityColumnValue) + 
										gamma_chunk * (prevChunkRowValue + prevChunkColumnValue));
					board[i][j] = colour;

				}

			}
		}

		return alpha;

	}



	void play()
	{


		if(	playerType == "CHAOS")
		{
				
			while( NoOfTurns > 0)
			{
				//1... Initially wait for colour
				if (NoOfTurns < 30)
				{
					MAXDEPTH = 4;
				}
				// else
				// MAXDEPTH = 3;

				gamma1 = weightBackup[NoOfTurns-1][0].g1;
				gamma_mobility = weightBackup[NoOfTurns-1][0].g2;
				gamma_chunk = weightBackup[NoOfTurns-1][0].g3;

				if(NoOfTurns <= 10)
				{
					gamma1 = 1;
					gamma_mobility = 0;
					gamma_chunk = 0;
					MAXDEPTH = 5;
				}

				if(NoOfTurns == gamma_turns)
				{
					gamma_mobility = gamma_update;
				}
				

				char inputcolour;
				cin >> inputcolour;
				
				
				countForPositions = -1;
				final_value = ObjectiveFunctionInitialisation();
				double finalize = minval(-9999999999,99999999999,inputcolour-'A',0);
				counter[inputcolour-'A']--;
				total--;
				

				if( MAXDEPTH_BY_2 < NoOfTurns )
				{
					NoOfTurns--;
					board[position.first][position.second] = inputcolour;
					final_value = ObjectiveFunctionInitialisation();
					finalize = maxvalSecondary(-9999999999,99999999999,0);
					board[position.first][position.second] = '$';
					int k = 0;
					int stp = min(countForPositions,3);
					while(k < stp)
					{
						board[positions[k].first][positions[k].second] = inputcolour;
						final_value = ObjectiveFunctionInitialisation();
						double valueK = maxvalSecondary(-9999999999,99999999999,0);
						board[positions[k].first][positions[k].second] = '$';
						if(valueK < finalize)
						{
							finalize = valueK;
							position = positions[k];
						}
						k++;
					}
				}
				else
				{
					NoOfTurns--;
				}


				cout << position.first << " " << position.second << endl;
				
				board[position.first][position.second] = inputcolour;
				// double newVal1 = ObjectiveFunctionInitialisation();

				int c,d,e,f;
				
				cin >> c >> d >> e >> f;

				char temp = board[c][d];
				board[c][d] = '$';
				board[e][f] = temp;

				double newVal2 = ObjectiveFunctionInitialisation();
				weights featureVals;

				featureVals.g1 = getFeature1(); 
				featureVals.g2 = getFeature2();
				featureVals.g3 = getFeature3();

				if (NoOfTurns <= 10)
				{
					featureVals.g2 = 0;
					featureVals.g3 = 0;
				}

				// changeWeights( 0 , NoOfTurns , LEARNING , newVal2 - finalize , featureVals );
				
				cerr<<"Index :"<<NoOfTurns<<endl;
				cerr<<"FeatureVals :"<<featureVals.g1<<"  "<<featureVals.g2<<"  "<<featureVals.g3<<endl;
				cerr<<"NewVals :"<<weightBackup[NoOfTurns][0].g1<<"  "<<weightBackup[NoOfTurns][0].g2<<"  "<<weightBackup[NoOfTurns][0].g3<<endl;
				cerr<<"difference : "<<finalize<<"  "<<newVal2<<endl;

				
						
			}
		}
		else if(playerType == "ORDER")
		{	
			while( NoOfTurns > 0 )
			{
				//2... kind of input

				// if(NoOfTurns == MAXDEPTH_BY_2)
				// cerr<<"NoOfTurns :"<<NoOfTurns<<endl;
				gamma1 = weightBackup[NoOfTurns-1][1].g1;
				gamma_mobility = weightBackup[NoOfTurns-1][1].g2;
				gamma_chunk = weightBackup[NoOfTurns-1][1].g3;

				
				
				if(NoOfTurns <= 20)
				{
					gamma1 = 1;
					gamma_mobility = 0;
					gamma_chunk = 0;
					MAXDEPTH = 4;
				}

				if(NoOfTurns <= gamma_turns)
				{
					gamma_mobility = gamma_update;
					MAXDEPTH = 5;
				}

				char inputcolour;
				int posX , posY;
				cin >> posX >> posY;
				cin >> inputcolour;
				
				board[posX][posY] = inputcolour;
				counter[inputcolour-'A']--;
				total--;

				double newVal2 = ObjectiveFunctionInitialisation();
				cerr<<"newVal2 :"<<newVal2<<endl;
				
				if (NoOfTurns != 36)
				{
						
						weights featureVals;
						featureVals.g1 = getFeature1(); 
						featureVals.g2 = getFeature2();
						featureVals.g3 = getFeature3();

						if (NoOfTurns < 10)
						{
							featureVals.g2 = 0;
							featureVals.g3 = 0;
						}

						cerr<<"PrevWeights1 :"<<weightBackup[NoOfTurns][1].g1<<"  "<<weightBackup[NoOfTurns][1].g2<<"  "<<weightBackup[NoOfTurns][1].g3<<endl;
						// changeWeights( 1 , NoOfTurns , LEARNING*0.1 , newVal2 - prevVal , featureVals );
						
						cerr<<"Index1 :"<<NoOfTurns<<endl;
						cerr<<"prevVal :"<<prevVal<<endl;
						cerr<<"FeatureVals1 :"<<featureVals.g1<<"  "<<featureVals.g2<<"  "<<featureVals.g3<<endl;
						cerr<<"NewWeights1 :"<<weightBackup[NoOfTurns][1].g1<<"  "<<weightBackup[NoOfTurns][1].g2<<"  "<<weightBackup[NoOfTurns][1].g3<<endl;
						cerr<<"difference1 : "<<prevVal<<"  "<<newVal2<<endl;

				}


				// prevVal = newVal2;
				// cerr<<"prevVal :"<<prevVal<<endl;
				countForPositions = -1;
				final_value = ObjectiveFunctionInitialisation();
				prevVal =  maxval(-9999999999,99999999999,0);
				

				if( MAXDEPTH_BY_2 < NoOfTurns )
				{
					NoOfTurns--;
					inputcolour = board[fromForOrder.first][fromForOrder.second]; 
					board[fromForOrder.first][fromForOrder.second] = '$';
					board[position.first][position.second] = inputcolour;
					final_value = ObjectiveFunctionInitialisation();
					double finalize = 0.0;
					for( int c = 0 ; c < N ; c++)
					{
						if(counter[c] > 0)
						{
							finalize += (double)(counter[c]) / (double)total * minvalSecondary(-9999999999,99999999999,(char)c,0) ;
						}
					}
					board[position.first][position.second] = '$';
					board[fromForOrder.first][fromForOrder.second] = inputcolour;
					int k = 0;
					int stp = min(countForPositions,3);
					while(k < stp)
					{
						inputcolour = board[fromForOrders[k].first][fromForOrders[k].second]; 
						board[fromForOrders[k].first][fromForOrders[k].second] = '$';
						board[positions[k].first][positions[k].second] = inputcolour;
						final_value = ObjectiveFunctionInitialisation();
						double valueK = 0.0;
						for( int c = 0 ; c < N ; c++)
						{
							if(counter[c] > 0)
							{
								valueK += (double)(counter[c]) / (double)total * minvalSecondary(-9999999999,99999999999,(char)c,0) ;
							}
						}
						board[positions[k].first][positions[k].second] = '$';
						board[fromForOrders[k].first][fromForOrders[k].second] = inputcolour;
						if(valueK > finalize)
						{
							finalize = valueK;
							position = positions[k];
							fromForOrder = fromForOrders[k];
						}
						k++;
					}
					prevVal = finalize;
				}
				else
				{
					NoOfTurns--;
				}



				inputcolour = board[fromForOrder.first][fromForOrder.second]; 
				board[fromForOrder.first][fromForOrder.second] = '$';
				board[position.first][position.second] = inputcolour;

				
				cout << fromForOrder.first << " " << fromForOrder.second << " " << position.first << " " << position.second<<endl;
				
			}

		}

	}

};






void initBackup(int N)
{
	ofstream f("backup.txt");

	for (int i = 0; i < N; ++i)
	{
		f << "7 1 2\n";
		f << "7 1 2\n";
	}
}

void printArray()
{
	for (int i = 0; i < 49; ++i)
	{
		cerr<<weightBackup[i][0].g1<<"  "<<weightBackup[i][0].g2<<"  "<<weightBackup[i][0].g3<<endl;
		cerr<<weightBackup[i][1].g1<<"  "<<weightBackup[i][1].g2<<"  "<<weightBackup[i][1].g3<<endl;
	}
}

int main(int argc, char const *argv[])
{
	/* code */
	string playerType;
	int N;
	cin>>N;
	cin>>playerType;
	// initBackup(N*N);


	if(N == 6)
	{
		readBackup6(N*N);
		printArray();

		Bot6 myPlayer(playerType,N,gamma1,gamma_mobility,gamma_chunk,gamma_turns,gamma_update);
		cerr<<"NEW\n";
		cerr << "Bot successfully created" << "\n\n";
		myPlayer.play();
		printArray();
	}

	//writeBackup(N*N);

	return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
  // Verifica se o número de argumentos é correto
  if (argc != 5) {
    cout << "Uso: ./matriz_auxiliar n1 n2 m1 m2" << endl;
    return 1;
  }

  // Lê os argumentos
  int n1 = atoi(argv[1]);
  int n2 = atoi(argv[2]);
  int m1 = atoi(argv[3]);
  int m2 = atoi(argv[4]);

  srand(time(0)); 

  // Declara as matrizes
  //long long unsigned int M[m1][m2];
  //long long unsigned int N[n1][n2];
  vector<vector<double>> M(m1, vector<double>(m1));
  vector<vector<double>> N(m1, vector<double>(m1));

  // Preenche as matrizes com números aleatórios
  for (int i = 0; i < m1; i++) {
    for (int j = 0; j < m2; j++) {
      double random_number = rand() % 100;
      M[i][j] = rand() % 100;
    }
  }

  for (int i = 0; i < n1; i++) {
    for (int j = 0; j < n2; j++) {
      double random_number = rand() % 100;
      N[i][j] = rand() % 100;
    }
  }

  // Grava a matriz M em um arquivo
  ofstream arquivoM("matrizM.txt");
  arquivoM << m1 << endl;
  for (int i = 0; i < m1; i++) {
    for (int j = 0; j < m2; j++) {
      arquivoM << M[i][j] << " ";
    }
    arquivoM << endl;
  }
  arquivoM.close();

  // Grava a matriz N em um arquivo
  ofstream arquivoN("matrizN.txt");
  arquivoN << m1 << endl;
  for (int i = 0; i < n1; i++) {
    for (int j = 0; j < n2; j++) {
      arquivoN << N[i][j] << " ";
    }
    arquivoN << endl;
  }
  arquivoN.close();

  return 0;
}

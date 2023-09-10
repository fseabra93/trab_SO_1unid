#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

// Function to multiply two matrices A and B
vector<vector<double>> multiply_matrices(vector<vector<double>> A, vector<vector<double>> B) {
  int m = A.size();
  int n = B[0].size();
  vector<vector<double>> C(m, vector<double>(n, 0));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < B.size(); k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  return C;
}

double getExecutionTime(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
}

void gravarArquivo(const vector<vector<double>>& result, double tempo) {
  
    int rows = result.size();

      fstream arquivoMatriz;
  //cria o arquivo matrizC.txt caso não exista
  arquivoMatriz.open("matrizC.txt", ios::out | ios::app);
  arquivoMatriz << rows << " " << rows << endl;
  for(int i=0; i<rows; i++){
    for (int j=0; j<rows; j++){
      arquivoMatriz << "c" << i <<  j<< " " << result[i][j] << endl;
    }  
  }
  arquivoMatriz << tempo*1000 << endl << endl;
  arquivoMatriz.close();

}

int main(int argc, char *argv[]) {
  // Open the first file and read the matrix size and values
  ifstream infileA(argv[1]);
  double sizeA;
  infileA >> sizeA;
  vector<vector<double>> A(sizeA, vector<double>(sizeA));
  for (int i = 0; i < sizeA; i++) {
    for (int j = 0; j < sizeA; j++) {
      infileA >> A[i][j];
    }
  }
  infileA.close();

  // Open the second file and read the matrix size and values
  ifstream infileB(argv[2]);
  double sizeB;
  infileB >> sizeB;
  vector<vector<double>> B(sizeB, vector<double>(sizeB));
  for (int i = 0; i < sizeB; i++) {
    for (int j = 0; j < sizeB; j++) {
      infileB >> B[i][j];
    }
  }
  infileB.close();

 /* // Print the matrices A and B
  cout << "Matrix A:" << endl;
  for (int i = 0; i < sizeA; i++) {
    for (int j = 0; j < sizeA; j++) {
      cout << A[i][j] << " ";
    }
    cout << endl;
  }

  cout << "Matrix B:" << endl;
  for (int i = 0; i < sizeB; i++) {
    for (int j = 0; j < sizeB; j++) {
      cout << B[i][j] << " ";
    }
    cout << endl;
  }*/

  // Multiply the matrices A and B and measure the time taken
  
  auto start_time = std::chrono::steady_clock::now(); // Inicia a contagem do tempo

  vector<vector<double>> C = multiply_matrices(A, B);

  auto end_time = std::chrono::steady_clock::now(); // Finaliza a contagem do tempo
  // Calcula e imprime o tempo de execução
  double execution_time = getExecutionTime(start_time, end_time);
    std::cout << "Tempo de execução: " << execution_time*1000 << " milissegundos" << std::endl;

 /* cout << "Matrix C:" << endl;
  for (int i = 0; i < sizeA; i++) {
    for (int j = 0; j < sizeB; j++) {
      cout << C[i][j] << " ";
    }
    cout << endl;
  }*/

    gravarArquivo(C, execution_time);

  return 0;
}
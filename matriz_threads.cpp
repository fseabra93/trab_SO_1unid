#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

vector<vector<double>> A;
vector<vector<double>> B;
vector<vector<double>> C;
vector<double> tempo;

//////////////////////////////////////////////////////////////

double getExecutionTime(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
}

void multiplyAndWriteResult(int threadId, int start, int end, int tam) {
    std::string filename = "thread_" + std::to_string(threadId) + ".txt";
    std::ofstream outputFile(filename);
    outputFile << tam << " " << tam << endl;

    auto start_time = std::chrono::steady_clock::now(); // Inicia a contagem do tempo

    for (int i = start; i < end; ++i) {
        int row = i / tam;
        int col = i % tam;
        C[row][col] = 0.0;

        for (int k = 0; k < tam; ++k) {
            C[row][col] += A[row][k] * B[k][col];
        }

        // Escreve o resultado no arquivo
        outputFile << "c" << row << col << " " << C[row][col] << endl;
    }

    auto end_time = std::chrono::steady_clock::now(); // Finaliza a contagem do tempo
    double execution_time = getExecutionTime(start_time, end_time);
    outputFile << "Tempo de execução: " << execution_time*1000 << " milissegundos" << endl;
    tempo.push_back(execution_time);
    outputFile.close();
}
    


int main(int argc, char* argv[]) {
  int P = atoi(argv[3]);
  // Open the first file and read the matrix size and values
  ifstream infileA(argv[1]);
  double sizeA;
  infileA >> sizeA;
  A.resize(sizeA, std::vector<double>(sizeA));
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
  B.resize(sizeA, std::vector<double>(sizeA));
  for (int i = 0; i < sizeB; i++) {
    for (int j = 0; j < sizeB; j++) {
      infileB >> B[i][j];
    }
  }
  infileB.close();
  C.resize(sizeA, std::vector<double>(sizeA));

    // Calcula o número de threads com base em P
    int tamA = (int)sizeA;
    int numThreads = sizeA / P;
    if ((tamA * tamA) % P != 0) {
        numThreads++;
    }

  ////////////// threads /////////////////////
    vector<thread> threads;

    int particao = tamA * tamA / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * particao;
        int end = (i == numThreads - 1) ? tamA * tamA : (i + 1) * particao;
        threads.emplace_back(multiplyAndWriteResult, i, start, end, tamA);
    }

    // Aguarda todas as threads terminarem
    for (auto& thread : threads) {
        thread.join();
    }

    double maior = 0.0;
    for (double tempo : tempo) {
        if (tempo > maior){
          maior = tempo;
        }
    }
    std::string filename = "tempo_medio_threads.txt";
    std::ofstream outputFile(filename);
    outputFile << "Maior tempo (threads): " << maior*1000 << " milissegundos" << endl;
    cout << "Maior tempo (threads): " << maior*1000 << " milissegundos" << endl;
    


    return 0;
}
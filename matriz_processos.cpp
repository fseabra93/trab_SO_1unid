#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
using namespace std;

vector<vector<double>> A;
vector<vector<double>> B;
vector<vector<double>> C;

struct SharedMemory {
    double maiorExecutionTime;
};

//////////////////////////////////////////////////////////////

double getExecutionTime(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
}

void multiplyAndWriteResult(int processId, int start, int end, int tam, SharedMemory* sharedMemory) {
    std::string filename = "processo_" + std::to_string(processId) + ".txt";
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
    if (execution_time > sharedMemory->maiorExecutionTime ){
        sharedMemory->maiorExecutionTime = execution_time;
    }
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
    int numProcesses = sizeA / P;
    if ((tamA * tamA) % P != 0) {
        numProcesses++;
    }

    // Cria e anexa o segmento de memória compartilhada
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        std::cerr << "Erro ao criar o segmento de memória compartilhada." << std::endl;
        return 1;
    }

    SharedMemory* sharedMemory = (SharedMemory*)shmat(shmid, nullptr, 0);

    // Inicializa o tempo total de execução compartilhado
    sharedMemory->maiorExecutionTime = 0.0;

  ////////////// processos /////////////////////
    vector<pid_t> processIds;

    int particao = tamA * tamA / numProcesses;

    for (int i = 0; i < numProcesses; ++i) {
        int start = i * particao;
        int end = (i == numProcesses - 1) ? tamA * tamA : (i + 1) * particao;
        pid_t childPid = fork();

        if (childPid == 0) {
            // Este é o processo filho
            multiplyAndWriteResult(getpid(), start, end, tamA, sharedMemory);
            shmdt(sharedMemory);
            exit(0);

        } else if (childPid > 0) {
            // Este é o processo pai
            processIds.push_back(childPid);
        } else {
            std::cerr << "Erro ao criar um processo." << std::endl;
            return 1;
        }


    }

    for (pid_t pid : processIds) {
        waitpid(pid, nullptr, 0);
    }

    
    std::string filename = "tempo_maior_processos.txt";
    std::ofstream outputFile(filename);
    outputFile << "Tempo maior (processos): " << sharedMemory->maiorExecutionTime*1000 << " milissegundos" << endl;
    outputFile.close();

    cout << "Tempo maior (processos): " << sharedMemory->maiorExecutionTime*1000 << " milissegundos" << endl;


    shmdt(sharedMemory);
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
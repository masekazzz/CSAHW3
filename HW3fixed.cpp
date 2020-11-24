#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>

using namespace std;


mutex mtx;
class Boy {
public:
    int number;

    string message;

    bool isLucky = false;

    Boy(int n) {
        number = n + 1;
        message = "Привет, я №" + to_string(number) + ". Пойдешь со мной на свидание?";
    }
};

bool isAgreed;


static void makeDecision(Boy &boy) {
    mtx.lock();
    if (!isAgreed) {
        cout << boy.message << endl;
        cout << "Введите 1, чтобы согласиться или 2, чтобы отказаться" << endl;
        int N;
        cin >> N;
        while (N > 2 || N < 1) {
            cout << "Неверный ввод, введите 1, чтобы согласиться или 2, чтобы отказаться" << endl;
            cin.clear();
            cin.ignore(32767, '\n');
            cin >> N;
        }
        if (N == 1)
            isAgreed = true;
        boy.isLucky = N == 1;
    } else
        boy.isLucky = false;
    mtx.unlock();
}

static void sendAnswer(const Boy &boy) {
    mtx.lock();
    if (boy.isLucky)
        cout << "Я выбираю тебя №" + to_string(boy.number) +"!" << endl;
    else
        cout << "Прости, №"+ to_string(boy.number) + ", дело не в тебе, дело во мне..." << endl;
    mtx.unlock();
}



int main() {
    vector<thread> decisionThreads;
    vector<Boy> boys;
    int N;

    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "Введите N > 0" << endl;
    cout << "> ";
    cin >> N;
    while (N < 1) {
        cout << "Неверный ввод, введите целое число больше нуля " << endl;
        cin.clear();
        cin.ignore(32767, '\n');
        cin >> N;
    }
    for (int i = 0; i < N; ++i) {
        boys.emplace_back(Boy(i));
    }
    for (int i = 0; i < N; ++i) {
        decisionThreads.emplace_back(makeDecision, ref(boys[i]));
    }
    for (int i = 0; i < N; ++i) {
        decisionThreads[i].join();
    }
    decisionThreads.clear();
    for (int i = 0; i < N; ++i) {
        decisionThreads.emplace_back(sendAnswer, boys[i]);
    }
    for (int i = 0; i < N; ++i) {
        decisionThreads[i].join();
    }
    return 0;
}
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;


mutex mtx;

void getAnswers(int number, int var)
{

    mtx.lock();
    if (var != number)
    {
        cout << "Извини, №" << number + 1 << " с тобой я не пойду." << endl;
    }
    else
    {
        cout << "А вот с тобой, №" << number + 1 << " я пойду." << endl;
    }
    mtx.unlock();
}

int main()
{
    vector<thread> boys;
    int number;
    int N;
    setlocale(LC_ALL, "rus");

    cout << "Введите N > 0" << endl;
    cout << "> ";
    cin >> N;
    while (N < 1) {
        cout << "Неверный ввод, введите целое число больше нуля " << endl;
        cin.clear();
        cin.ignore(32767, '\n');
        cin >> N;
    }
    cout << "Вы получили " << N << " валентинок. Какую выбираете?:" << endl;

    for (int i = 0; i < N; i++)
        cout << "Привет, я №" << i + 1 << ". Пойдешь со мной на свидание?" << endl;

    cout << "> ";
    cin >> number;
    while (number < 1 || number > N) {
        cout << "Такой валентинки нет. Введите существующий номер валентинки" << endl;
        cout << "> ";
        cin.clear();
        cin.ignore(32767, '\n');
        cin >> number;
    }
    cout << endl;
    for (int i = 0; i < N; i++)
        boys.push_back(thread(getAnswers, i, number - 1));
    for (int i = 0; i < N; i++)
        boys[i].join();

    return 0;
}
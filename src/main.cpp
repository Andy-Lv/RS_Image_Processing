#include"include/main.h"

int main()
{

	int choice;
	cout<<"��������Ҫ�Ĳ���"<<endl;

	while(true)
    {
	    //֤�����
        ShowLiscence();

	    //�˵�����
	    ShowMenu();
	    cin>>choice;

        switch (choice)
        {
            case 1:
                FirstProcessing();
                break;
            case 2:
                SecondProcessing();
                break;
            case 3:
                ThirdProcessing();
                break;
            case 0:
                return 0;
            default:
                cout<<"����������,����������"<<endl;
                system("pause");
                system("clear");
        }
    }
}
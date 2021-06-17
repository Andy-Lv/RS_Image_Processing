#include"include/main.h"

int main()
{

	int choice;
	cout<<"请输入需要的操作"<<endl;

	while(true)
    {
	    //证书操作
        ShowLiscence();

	    //菜单调用
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
                cout<<"您输入有误,请重新输入"<<endl;
                system("pause");
                system("clear");
        }
    }
}
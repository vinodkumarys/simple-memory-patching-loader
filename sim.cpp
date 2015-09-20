#include <windows.h>

int main()
{
  int a = 500;

  if(a==500)
      MessageBox(NULL,"This is  a unregistered copy.","Simple Program",MB_OK | MB_ICONERROR);
  else
      MessageBox(NULL,"This is a Registered Copy!","Simple Program",MB_OK | MB_ICONINFORMATION);
  return 0;
}

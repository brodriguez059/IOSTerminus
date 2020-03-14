#include <stdio.h>

int main()
{
	FILE *f;
	
	f=fopen("help.txt","r");
	
	if( f==NULL )
		printf("Error al abrir el fichero\n");
		
	else
	{
		while( !feof(f) )
			printf("%c",getc(f));
	}
}














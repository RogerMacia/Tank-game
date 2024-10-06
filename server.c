#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <mysql.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>



int main(int argc, char *argv[]) {
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query [100];
	char query2 [100];
	int gameId;
	
	//Creamos una conexion al servidor MYSQL
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Tanks",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char answer[512];
	
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(9000);
	
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	for (;;) {
		printf("Listening...\n");
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexion\n");
		
		ret = read(sock_conn, peticion, sizeof(peticion));
		printf("Recibido\n");
		
		peticion[ret] = '\0';
		
		printf("Peticion: %s\n", peticion);
		
		char *p = strtok(peticion, "/");
		int code = atoi(p);
		p = strtok(NULL, "/");
		
		char name[20];
		strcpy(name, p);
		
		printf("Codigo: %d, Nombre: %s\n", code, name);
		
		if (code == 1) {
			// consulta SQL para obtener una tabla con todos los datos
			// de la base de datos
			
			strcpy(query, "SELECT DISTINCT Game.id_Map FROM (Player, Game, Participant) WHERE Player.name = '");
			strcat(query, name);
			
			strcpy(query2, "' AND Player.id = Participant.id_P AND Participant.id_G = Game.id");
			strcat(query, query2);
			
			err=mysql_query (conn, query);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
			}
			
			//recogemos el resultado de la consulta. El resultado de la
			//consulta se devuelve en una variable del tipo puntero a
			//MYSQL_RES tal y como hemos declarado anteriormente.
			//Se trata de una tabla virtual en memoria que es la copia
			//de la tabla real en disco.
			res = mysql_store_result (conn);
			// El resultado es una estructura matricial en memoria
			// en la que cada fila contiene los datos de una persona.
			// Ahora obtenemos la primera fila que se almacena en una
			// variable de tipo MYSQL_ROW
			row = mysql_fetch_row (res);
			// En una fila hay tantas columnas como datos tiene
			
			if (row == NULL)
				printf ("No se han obtenido datos en la consulta\n");
			else {
				printf ("Map IDs:\n");
				printf ("%s\n", row[0]);
				strcpy(answer, row[0]);
				strcat(answer, ", ");
				// obtenemos la siguiente fila
				row = mysql_fetch_row (res);
				
				while (row !=NULL) {
					printf ("%s\n", row[0]);
					sprintf(answer, "%s%s, ", answer, row[0]);
					// obtenemos la siguiente fila
					row = mysql_fetch_row (res);
				}
				
				answer[strlen(answer) - 2] = '\0';
			}
		}
		
		printf("Respuesta: %s\n", answer);
		write(sock_conn, answer, strlen(answer));
		
		close(sock_conn);
	}
	
	mysql_close (conn);
	
	return 0;
}


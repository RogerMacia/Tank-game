#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <mysql.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
	char name[20];
	int socket;
} Tconnection;

typedef struct {
	Tconnection connections[50];
	int num;
} Tconnection_list;

int add_user(Tconnection_list *l, char name[20], int *socket) {
	if (l->num == 50) {
		return 1;
	}
	
	else {
		strcpy(l->connections[l->num].name, name);
		l->connections[l->num].socket = *socket;
		l->num ++;
		return 0;
	}
}

int delete_user(Tconnection_list *l, char name[20]) {
	int i;
	int found;
	
	for (i = 0; i < l->num; i++) {
		if (strcmp(l->connections[i].name, name)) {
			l->connections[i] = l->connections[l->num];
			strcpy(l->connections[l->num].name, "");
			l->connections[l->num].socket = 0;
			l->num --;
			return 0;
		}
	}
	
	return 1;
}

int get_socket(Tconnection_list *l, char name[20]) {
	for (int i = 0; i < l->num; i++) {
		if (strcmp(l->connections[i].name, name)) {
			return l->connections[i].socket;
		}
	}
	
	return 1;
}

int get_connection_list(Tconnection_list *l, char list[100]) {
	char socket_num[5];
	sprintf(socket_num, "%d", l->num);
	strcpy(list, socket_num);
	
	for (int i = 0; i < l->num; i++) {
		sprintf(list, "%s,%s", list, l->connections[i].name);
	}
	
	return 0;
}

void *AttendClient(void *socket) {
	int sock_conn;
	int *s;
	s = (int *) socket;
	sock_conn = *s;
	
	char query[512];
	char query2 [512];
	char answer[512];
	int ret;
	int err;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *p;
	int end_connection = 0;
	
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
	
	while (!end_connection) {
		ret = read(sock_conn, query, sizeof(query));
		printf("Recibido\n");
		
		query[ret] = '\0';
		
		printf("Peticion: %s\n", query);
		
		p = strtok(query, "/");
		int code = atoi(p);
		
		char name[20];
		
		printf("Codigo: %d\n", code);
		
		if (code == 1) {
			p = strtok(NULL, "/");
			strcpy(name, p);
			// consulta SQL para obtener una tabla con todos los datos
			// de la base de datos
			
			strcpy(query, "SELECT DISTINCT Game.id_Map FROM (Player, Game, Participant) WHERE Player.name = '");
			strcat(query, name);
			
			strcpy(query2, "' AND Player.id = Participant.id_P AND Participant.id_G = Game.id");
			strcat(query, query2);
			
			err = mysql_query (conn, query);
			
			if (err != 0) {
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
			
			if (row == NULL) {
				printf("No se han obtenido datos en la consulta\n");
				strcpy(answer, "No se han obtenido datos en la consulta");
			}
			
			else {
				printf("Map IDs:\n");
				printf("%s\n", row[0]);
				
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
			
			printf("Respuesta: %s\n", answer);
			write(sock_conn, answer, strlen(answer));
		}
		
		else if (code == 0) {
			end_connection = 1;
		}
		
		else {
			strcpy(answer, "Error at the code");
			write(sock_conn, answer, strlen(answer));
		}
	}
	
	close(sock_conn);
	mysql_close (conn);
}

int main(int argc, char *argv[]) {
	
	// Estructura especial para almacenar resultados de consultas
	Tconnection_list connection_list;
	connection_list.num = 0;
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket\n");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(9080);
	
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf("Error al bind\n");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen\n");
	
	pthread_t thread[100];
	
	for (int i = 0;;i++) {
		printf("Listening...\n");
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido conexion\n");
		
		add_user(&connection_list, "A", &sock_conn);
		
		printf("\t%s\n\t%d\n", connection_list.connections[connection_list.num - 1].name, connection_list.connections[connection_list.num - 1].socket);
		
		pthread_create(&thread[i], NULL, AttendClient, &connection_list.connections[connection_list.num - 1].socket);
	}
	
	return 0;
}


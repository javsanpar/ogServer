// **************************************************************************************************************************************************
// Libreria: ogAdmLib
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Marzo-2010
// Fecha Última modificación: Marzo-2010
// Nombre del fichero: ogAdmLib.c
// Descripción: Este fichero implementa una libreria de funciones para uso común de los servicios
// **************************************************************************************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ogAdmLib.h"

char szPathFileCfg[4096],szPathFileLog[4096];
int ndebug;

//______________________________________________________________________________________________________
// Función: ValidacionParametros
//
//	 Descripción:
//		Valida que los parametros de ejecución del programa sean correctos
//	Parámetros:
//		- argc:	Número de argumentos
//		- argv:	Puntero a cada argumento
//		- eje:	Tipo de ejecutable (1=Servicio,2=Repositorio o 3=Cliente)
//	Devuelve:
//		- TRUE si los argumentos pasados son correctos
//		- FALSE en caso contrario
//	Especificaciones:
//		La sintaxis de los argumentos es la siguiente
//			-f	Archivo de configuración del servicio
//			-l	Archivo de logs
//			-d	Nivel de debuger (mensages que se escribirán en el archivo de logs)
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
//______________________________________________________________________________________________________
BOOLEAN validacionParametros(int argc, char*argv[],int eje) {
	int i;

	switch(eje){
		case 1: // Administrador
			strcpy(szPathFileCfg, "ogserver.cfg"); // Valores por defecto de archivos
			strcpy(szPathFileLog, "ogserver.log"); // de configuración y de logs
			break;
		case 2: // Repositorio
			strcpy(szPathFileCfg, "ogAdmRepo.cfg"); // Valores por defecto de archivos
			strcpy(szPathFileLog, "ogAdmRepo.log"); // de configuración y de logs
			break;
		case 3: // Cliente OpenGnsys
			strcpy(szPathFileCfg, "ogAdmClient.cfg"); // Valores por defecto de archivos
			strcpy(szPathFileLog, "ogAdmClient.log"); // de configuración y de logs
			break;
		case 4: // Servicios DHCP,BOOTP Y TFTP
			strcpy(szPathFileCfg, "ogAdmBoot.cfg"); // Valores por defecto de archivos
			strcpy(szPathFileLog, "ogAdmBoot.log"); // de configuración y de logs
			break;
		case 5: // Agente
			strcpy(szPathFileCfg, "ogAdmAgent.cfg"); // Valores por defecto de archivos
			strcpy(szPathFileLog, "ogAdmAgent.log"); // de configuración y de logs
			break;
		case 6: // Agente
			strcpy(szPathFileCfg, "ogAdmWinClient.cfg"); // Valores por defecto de archivos
			strcpy(szPathFileLog, "ogAdmWinClient.log"); // de configuración y de logs
			break;
		case 7: // Agente
			strcpy(szPathFileCfg, "ogAdmnxClient.cfg"); // Valores por defecto de archivos
			strcpy(szPathFileLog, "ogAdmLnxClient.log"); // de configuración y de logs
			break;
	}

	ndebug = 1; // Nivel de debuger por defecto

	for (i = 1; (i + 1) < argc; i += 2) {
		if (argv[i][0] == '-') {
			switch (tolower(argv[i][1])) {
			case 'f':
				if (argv[i + 1] != NULL)
					strcpy(szPathFileCfg, argv[i + 1]);
				else {
					return (FALSE);
				}
				break;
			case 'l':
				if (argv[i + 1] != NULL)
					strcpy(szPathFileLog, argv[i + 1]);
				else {
					return (FALSE);
				}
				break;
			case 'd':
				if (argv[i + 1] != NULL) {
					ndebug = atoi(argv[i + 1]);
					if (ndebug < 1)
						ndebug = 1; // Por defecto el nivel de debug es 1
				} else
					ndebug = 1; // Por defecto el nivel de debug es 1
				break;
			default:
				exit(EXIT_FAILURE);
				break;
			}
		}
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: splitCadena
//
//	Descripción:
//			Trocea una cadena según un carácter delimitador
//	Parámetros:
//			- trozos: Array de punteros a cadenas
//			- cadena: Cadena a trocear
//			- chd: Carácter delimitador
//	Devuelve:
//		Número de trozos en que se divide la cadena
// ________________________________________________________________________________________________________
int splitCadena(char **trozos,char *cadena, char chd)
{
	int w=0;
	if(cadena==NULL) return(w);

	trozos[w++]=cadena;
	while(*cadena!='\0'){
		if(*cadena==chd){
			*cadena='\0';
			if(*(cadena+1)!='\0')
				trozos[w++]=cadena+1;
		}
		cadena++;
	}
	return(w); // Devuelve el número de trozos
}
// ________________________________________________________________________________________________________
// Función: escaparCadena
//
//	Descripción:
//			Sustituye las apariciones de un caracter comila simple ' por \'
//	Parámetros:
//			- cadena: Cadena a escapar
// Devuelve:
//		La cadena con las comillas simples sustituidas por \'
// ________________________________________________________________________________________________________
char* escaparCadena(char *cadena)
{
	int b,c;
	char *buffer;

	buffer = (char*) reservaMemoria(strlen(cadena)*2); // Toma memoria para el buffer de conversión
	if (buffer == NULL) { // No hay memoria suficiente para el buffer
		return (FALSE);
	}

	c=b=0;
	while(cadena[c]!=0) {
		if (cadena[c]=='\''){
			buffer[b++]='\\';
			buffer[b++]='\'';
		}
		else{
			buffer[b++]=cadena[c];
		}
		c++;
	}
	return(buffer);
}

// ________________________________________________________________________________________________________
// Función: igualIP
//
//	Descripción:
//		Comprueba si una cadena con una dirección IP está incluida en otra que	contienen varias direcciones ipes
//		separadas por punto y coma
//	Parámetros:
//		- cadenaiph: Cadena de direcciones IPES
//		- ipcliente: Cadena de la IP a buscar
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN contieneIP(char *cadenaiph,char *ipcliente)
{
	char *posa,*posb;
	int lon, i;

	posa=strstr(cadenaiph,ipcliente);
	if(posa==NULL) return(FALSE); // No existe la IP en la cadena
	posb=posa; // Iguala direcciones
	for (i = 0; i < LONIP; i++) {
		if(*posb==';') break;
		if(*posb=='\0') break;
		if(*posb=='\r') break;
		posb++;
	}
	lon=strlen(ipcliente);
	if((posb-posa)==lon) return(TRUE); // IP encontrada
	return(FALSE);
}
// ________________________________________________________________________________________________________
// Función: rTrim
//
//		 Descripción:
//			Elimina caracteres de espacios y de asci menor al espacio al final de la cadena
//		Parámetros:
//			- cadena: Cadena a procesar
// ________________________________________________________________________________________________________
char* rTrim(char *cadena)
{
	int i,lon;

	lon=strlen(cadena);
	for (i=lon-1;i>=0;i--){
		if(cadena[i]<32)
			cadena[i]='\0';
		else
			return(cadena);
	}
	return(cadena);
}
//______________________________________________________________________________________________________
// Función: reservaMemoria
//
//	Descripción:
//		Reserva memoria para una variable
//	Parámetros:
//		- lon:	Longitud en bytes de la reserva
//	Devuelve:
//		Un puntero a la zona de memoria reservada que ha sido previamente rellena con zeros o nulos
//______________________________________________________________________________________________________
char* reservaMemoria(int lon)
{
	char *mem;

	mem=(char*)malloc(lon);
	if(mem!=NULL)
		memset(mem,0,lon);
	return(mem);
}

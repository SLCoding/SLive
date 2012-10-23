//
//  CDatabase_Connection.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//

#ifndef __SLive__CDatabase_Connection__
#define __SLive__CDatabase_Connection__

#include <iostream>
#include <string>
#include <map>
#include <mysql.h>

using namespace std;

class CDatabase_Connection
{
private:
    MYSQL *my;					// Handle einer Datenbankverbindung
    MYSQL_ROW  row;				// Eine typensichere Darstellung eines Datensatzes (Zeile)
    MYSQL_RES  *mysql_res;		// Ergebnis einer Anfrage, welche Zeilen zur¸ckgibt (SELECT,
        // SHOW, DESCRIBE,EXPLAIN)
    MYSQL_FIELD  *field;		// Eine Struktur mit Informationen ¸ber ein Feld,
        // Feldnamen, Feldtyp und Feldgrˆﬂe
    string username;			// Benutzername zum Einloggen
    string password;			// Passwort des Benutzers
    string host;				// Hostname des MySQL Servers oder IP
    string db;					// Datenbankname
    int port;					// Portnummer
    string socket_name;			// Entweder NULL oder ein String, der ein Socket oder Named
        // Pipe festlegt; der Verbindungstyp wird dabei vom
        // host-Parameter festgelegt
    int flag;					// In der Regel wird hierf¸r der Wert 0 angegeben. Es sind aber
        // auch spezielle Optionen, einzeln oder kombiniert, mˆglich:
        // CLIENT_COMPRESS, CLIENT_FOUND_ROWS, CLIENT_IGNORE_SPACE,
        // CLIENT_INTER_ACTIVE, CLIENT_NO_SCHEMA, CLIENT_ODBC,
        // CLIENT_SSL
    unsigned int  i, col_len;
    bool connected;				// true, wenn eine Verbindung zu einem MySQL-Server aufgebaut ist
    bool initialised;			// true, wenn eine Abfrage abgesendet wurde
public:
    // Konstruktor / Destruktor
    CDatabase_Connection();
    virtual ~CDatabase_Connection();
    // Elementfunktionen
    void setUsername(string);		// Setzt Benuternamen
    void setPassword(string);		// Setzt Passwort
    void setHost(string);			// Setzt Host / IP
    void setDB(string);				// Setzt Namen der Datenbank
    void setPort(int);				// Setzt Portnummer
    void setSocket(string);
    void setFlag(int);
    string GetUsername();
    string GetPassword();
    string GetHost();
    string GetDB();
    int GetPort();
    string GetSocket();
    int GetFlag();
    // Datenbank-Funktionen
    bool connect(void);					// Baut Verbindung zur Datenbank auf
    bool close(void);					// Beendet Verbindung mit der Datenbank
    bool query(string,unsigned int);			// Sendet SQL Befehl an die Datenbank
    void LoadDefaults();				// Setzt alle Einstellungen zur¸ck
    bool UseDB(string);					// Wechselt die aktuelle Datenbank
    map<string, string> fetch_assoc();	// Liefert die aktuelle Datenzeile zur¸ck
    
		
    // API-Funktionen
    
    my_ulonglong affected_rows(/*MYSQL**/);				// Liefert die Anzahl der mit dem letzten UPDATE, DELETE oder INSERT ge‰nderten/gelˆschten/eingef¸gten Zeilen
    my_bool autocommit(/*MYSQL**/ bool);			// Schaltet Autocommit ein oder aus(1 = ein, 0 = aus); R¸ckgabewert == 0 => Fehler
		//change_user() 							// ƒndert den Benutzer und die Datenbank f¸r eine offene Verbindung
    string character_set_name(/*MYSQL**/);		// Gibt den Standardzeichensatz f¸r die aktuelle Verbindung zur¸ck.
    my_bool commit(/*MYSQL**/); 					// Committet die Transaktion. Was diese Funktion tut, h‰ngt von dem Wert der Systemvariablen completion_type ab. Wenn der completion_type 2 ist, gibt der Server nach Beendigung der Transaktion Ressourcen frei und schlieﬂt die Clientverbindung. Das Clientprogramm sollte mysql_close() aufrufen, um die Verbindung von der Clientseite aus zu schlieﬂen.
    string error();
    int errnum();
		//??data_seek() 							// Sucht in der Ergebnismenge einer Anfrage eine beliebige Datenzeile
    void debug(string debug); 					// F¸hrt ein DBUG_PUSH mit dem angegebenen String durch
    int dump_debug_info(/*MYSQL**/); 			//L‰sst den Server Debugginginformationen in das Log schreiben, benˆtigt Superrechte; R¸ckgabewert == 0 => OK!
    unsigned long real_escape_string(/*MYSQL**/ string to, const string from, unsigned long length); 					// Versieht Sonderzeichen in einem String, der in einer SQL-Anweisung benutzt werden soll, mit Escape-Symbolen
    unsigned int field_count(); 				// Liefert die Anzahl der Ergebnisspalten der letzten Anweisung
    void free_result(/*MYSQL_RES **/); 			// Gibt den von einer Ergebnismenge belegten Speicher frei
    string get_client_info(); 					// Liefert Versionsinformationen f¸r den Client als String
    unsigned long get_client_version();			// Liefert einen Integer, der die Version der Clientbibliothek darstellt. Der Wert hat das Format XYYZZ, wobei X die Hauptversion, YY der Release-Level und ZZ die Versionsnummer innerhalb des Releases ist. So bedeutet beispielsweise der Wert 40102 die Clientbibliotheksversion 4.1.2.
    string get_host_info(/*MYSQL **/); 			// Liefert einen String, der die Verbindung beschreibt
    unsigned long get_server_version(/*MYSQL **/); //Liefert die Versionsnummer des Servers als Integer.
    unsigned int get_proto_info(/*MYSQL **/); 	// Liefert die Protokollversion der Verbindung
    string get_server_info(/*MYSQL **/); 		// Liefert die Versionsnummer des Servers
    string info(/*MYSQL **/); 					// Liefert Informationen ¸ber die zuletzt ausgef¸hrte Anfrage
    my_ulonglong insert_id(/*MYSQL **/); 				// Liefert die f¸r eine AUTO_INCREMENT-Spalte von der vorhergehenden Anfrage generierte ID
    int kill(unsigned long pid); 				// H‰lt einen gegebenen Thread an, 0 bei Erfolg
		// !!! list_dbs()							benutze SHOW DATABASES [LIKE wild]
		// !!! list_fields() 						SHOW COLUMNS FROM tbl_name anstelle von mysql_list_fields()
		// !!! list_processes() 					// Liefert eine Liste der aktuellen Server-Threads ==> SHOW PROCESSLIST
		// !!! list_tables() 						// Liefert Tabellennamen, die mit einem einfachen regul‰ren Ausdruck ¸bereinstimmen	==> SHOW tables [LIKE wild].
    unsigned int num_fields(/*MYSQL_RES *result*/); 		// Liefert die Anzahl der Spalten einer Ergebnismenge
    my_ulonglong num_rows(/*MYSQL_RES *result*/); 			// Liefert die Anzahl der Zeilen einer Ergebnismenge
		// sp‰ter....options() 						// Stellt Verbindungsoptionen f¸r mysql_connect() ein
    int ping(/*MYSQL **/); 						// Pr¸ft, ob die Serververbindung funktioniert, und baut bei Bedarf eine neue Verbindung auf, 0 wenn OK
		//real_escape_string()						// Versieht Sonderzeichen in einem String, der in einer SQL-Anweisung benutzt werden soll, mit Escape-Symbolen, wobei der aktuelle Zeichensatz der Verbindung ber¸cksichtigt wird
    int refresh(/*MYSQL **/ unsigned int options); 			// Schreibt Tabellen und Caches auf die Platte zur¸ck oder setzt sie zur¸ck
		//reload();									L‰sst den Server die Berechtigungstabellen neu laden ==> FLUSH PRIVILEGES
    int shutdown(/*MYSQL **/ enum mysql_enum_shutdown_level shutdown_level); 			// F‰hrt den Datenbankserver herunter;http://dev.mysql.com/doc/refman/5.1/de/mysql-shutdown.html;
    string stat(/*MYSQL**/); 					// Liefert den Serverstatus als String	; 0 bei Fehler
    unsigned long thread_id(/*MYSQL**/); 		// Liefert die aktuelle Thread-ID
    unsigned int warning_count(/*MYSQL**/);		// Liefert die Anzahl der Warnungen f¸r die vorangehende SQL-Anweisung
};

#endif /* defined(__SLive__CDatabase_Connection__) */

    // Includefiles
    // Zus‰tzlich muss in das Projekt die libmysql.lib eingebunden werden. Die gleichnamige .dll muss im Projektverzeichnis sein!
    // Ich empfehle die benˆtigten Dateien aus dem mysql-Ordner einer XAMPP-Installation zu verwenden
    //#include "C:\xampp\MySQL\include\mysql.h"



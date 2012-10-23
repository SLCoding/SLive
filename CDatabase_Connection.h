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
#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class CDatabase_Connection
{
private:
    string username;			// Benutzername zum Einloggen
    string password;			// Passwort des Benutzers
    string host;				// Hostname des MySQL Servers oder IP
    string db;					// Datenbankname

    unsigned int col_len;
    bool connected;				// true, wenn eine Verbindung zu einem MySQL-Server aufgebaut ist
    bool initialised;			// true, wenn eine Abfrage abgesendet wurde
    
    const char *conninfo;
    PGconn     *conn;
    PGresult   *res;
    int         nFields;
    int         i;
    int         j;
protected:
    
public:
    CDatabase_Connection();
    virtual ~CDatabase_Connection();
    void setUsername(string);		// Setzt Benuternamen
    void setPassword(string);		// Setzt Passwort
    void setHost(string);			// Setzt Host / IP
    void setDB(string);				// Setzt Namen der Datenbank
    string GetUsername();
    string GetPassword();
    string GetHost();
    string GetDB();
    
    // Datenbank-Funktionen
    bool connect(void);					// Baut Verbindung zur Datenbank auf
    bool close(void);					// Beendet Verbindung mit der Datenbank
    bool query(string,unsigned int);			// Sendet SQL Befehl an die Datenbank
    void LoadDefaults();				// Setzt alle Einstellungen zur¸ck
    bool UseDB(string);					// Wechselt die aktuelle Datenbank
    map<string, string> fetch_assoc();	// Liefert die aktuelle Datenzeile zur¸ck
    
};

#endif /* defined(__SLive__CDatabase_Connection__) */


    
    //my_ulonglong affected_rows(/*MYSQL**/);				// Liefert die Anzahl der mit dem letzten UPDATE, DELETE oder INSERT ge‰nderten/gelˆschten/eingef¸gten Zeilen
    //my_bool autocommit(/*MYSQL**/ bool);			// Schaltet Autocommit ein oder aus(1 = ein, 0 = aus); R¸ckgabewert == 0 => Fehler
		//change_user() 							// ƒndert den Benutzer und die Datenbank f¸r eine offene Verbindung
    //    string character_set_name(/*MYSQL**/);		// Gibt den Standardzeichensatz f¸r die aktuelle Verbindung zur¸ck.
    //my_bool commit(/*MYSQL**/); 					// Committet die Transaktion. Was diese Funktion tut, h‰ngt von dem Wert der Systemvariablen completion_type ab. Wenn der completion_type 2 ist, gibt der Server nach Beendigung der Transaktion Ressourcen frei und schlieﬂt die Clientverbindung. Das Clientprogramm sollte mysql_close() aufrufen, um die Verbindung von der Clientseite aus zu schlieﬂen.
    //string error();
    //int errnum();
		//??data_seek() 							// Sucht in der Ergebnismenge einer Anfrage eine beliebige Datenzeile
    //void debug(string debug); 					// F¸hrt ein DBUG_PUSH mit dem angegebenen String durch
    //int dump_debug_info(/*MYSQL**/); 			//L‰sst den Server Debugginginformationen in das Log schreiben, benˆtigt Superrechte; R¸ckgabewert == 0 => OK!
    //unsigned long real_escape_string(/*MYSQL**/ string to, const string from, unsigned long length); 					// Versieht Sonderzeichen in einem String, der in einer SQL-Anweisung benutzt werden soll, mit Escape-Symbolen
    //    unsigned int field_count(); 				// Liefert die Anzahl der Ergebnisspalten der letzten Anweisung
    //void free_result(/*MYSQL_RES **/); 			// Gibt den von einer Ergebnismenge belegten Speicher frei
    //string get_client_info(); 					// Liefert Versionsinformationen f¸r den Client als String
    // unsigned long get_client_version();			// Liefert einen Integer, der die Version der Clientbibliothek darstellt. Der Wert hat das Format XYYZZ, wobei X die Hauptversion, YY der Release-Level und ZZ die Versionsnummer innerhalb des Releases ist. So bedeutet beispielsweise der Wert 40102 die Clientbibliotheksversion 4.1.2.
    // string get_host_info(/*MYSQL **/); 			// Liefert einen String, der die Verbindung beschreibt
    //unsigned long get_server_version(/*MYSQL **/); //Liefert die Versionsnummer des Servers als Integer.
    //unsigned int get_proto_info(/*MYSQL **/); 	// Liefert die Protokollversion der Verbindung
    //string get_server_info(/*MYSQL **/); 		// Liefert die Versionsnummer des Servers
    //string info(/*MYSQL **/); 					// Liefert Informationen ¸ber die zuletzt ausgef¸hrte Anfrage
    //my_ulonglong insert_id(/*MYSQL **/); 				// Liefert die f¸r eine AUTO_INCREMENT-Spalte von der vorhergehenden Anfrage generierte ID
    //int kill(unsigned long pid); 				// H‰lt einen gegebenen Thread an, 0 bei Erfolg
		// !!! list_dbs()							benutze SHOW DATABASES [LIKE wild]
		// !!! list_fields() 						SHOW COLUMNS FROM tbl_name anstelle von mysql_list_fields()
		// !!! list_processes() 					// Liefert eine Liste der aktuellen Server-Threads ==> SHOW PROCESSLIST
		// !!! list_tables() 						// Liefert Tabellennamen, die mit einem einfachen regul‰ren Ausdruck ¸bereinstimmen	==> SHOW tables [LIKE wild].
    //unsigned int num_fields(/*MYSQL_RES *result*/); 		// Liefert die Anzahl der Spalten einer Ergebnismenge
    //my_ulonglong num_rows(/*MYSQL_RES *result*/); 			// Liefert die Anzahl der Zeilen einer Ergebnismenge
		// sp‰ter....options() 						// Stellt Verbindungsoptionen f¸r mysql_connect() ein
    //    int ping(/*MYSQL **/); 						// Pr¸ft, ob die Serververbindung funktioniert, und baut bei Bedarf eine neue Verbindung auf, 0 wenn OK
		//real_escape_string()						// Versieht Sonderzeichen in einem String, der in einer SQL-Anweisung benutzt werden soll, mit Escape-Symbolen, wobei der aktuelle Zeichensatz der Verbindung ber¸cksichtigt wird
    //int refresh(/*MYSQL **/ unsigned int options); 			// Schreibt Tabellen und Caches auf die Platte zur¸ck oder setzt sie zur¸ck
		//reload();									L‰sst den Server die Berechtigungstabellen neu laden ==> FLUSH PRIVILEGES
    //    int shutdown(/*MYSQL **/ enum mysql_enum_shutdown_level shutdown_level); 			// F‰hrt den Datenbankserver herunter;http://dev.mysql.com/doc/refman/5.1/de/mysql-shutdown.html;
    //string stat(/*MYSQL**/); 					// Liefert den Serverstatus als String	; 0 bei Fehler
    //unsigned long thread_id(/*MYSQL**/); 		// Liefert die aktuelle Thread-ID
    //unsigned int warning_count(/*MYSQL**/);		// Liefert die Anzahl der Warnungen f¸r die vorangehende SQL-Anweisung
    //};


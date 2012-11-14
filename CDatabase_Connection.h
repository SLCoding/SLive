//
//  CDatabase_Connection.h
//  SLive
//
//  Created by Markus Scholl on 10.10.12.
//  Copyright (c) 2012 CLMM. All rights reserved.
//
//  http://www.macuser.de/forum/f25/xcode-c-mysql-629682/  INSTALL NOTES UNDER MAC FOR XCODE!!!
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
    MYSQL my;					// Handle einer Datenbankverbindung
    MYSQL_ROW  row;				// Eine typensichere Darstellung eines Datensatzes (Zeile)
    MYSQL_RES  *mysql_res;		// Ergebnis einer Anfrage, welche Zeilen zurückgibt (SELECT,
                                // SHOW, DESCRIBE,EXPLAIN)
    MYSQL_FIELD  *field;		// Eine Struktur mit Informationen über ein Feld,
                                // Feldnamen, Feldtyp und Feldgröße
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
    CDatabase_Connection();
    CDatabase_Connection(string user, string password, string DB, string Host = "127.0.0.1", int port = 3306);
    virtual ~CDatabase_Connection();
    CDatabase_Connection(const CDatabase_Connection& rhs); // usually rhs: "Right Hand Side"
    // Elementfunktionen
    void setUsername(string);           // Setzt Benuternamen
    void setPassword(string);           // Setzt Passwort
    void setHost(string);               // Setzt Host / IP
    void setDB(string);                 // Setzt Namen der Datenbank
    void setPort(int);                  // Setzt Portnummer
    void setSocket(string);             // Setzt den Socket
    void setFlag(int);                  // Setzt spezielle Option-Flags
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
    bool query(string,unsigned int);	// Sendet SQL Befehl an die Datenbank
    void LoadDefaults();				// Setzt alle Einstellungen zur¸ck
    bool UseDB(string);					// Wechselt die aktuelle Datenbank
    map<string, string> fetch_assoc();	// Liefert die aktuelle Datenzeile zurück

    // API-Funktionen
    my_ulonglong affected_rows();		// Liefert die Anzahl der mit dem letzten UPDATE, DELETE oder INSERT geänderten/gelöschten/eingefügten Zeilen
    my_bool autocommit(bool);           // Schaltet Autocommit ein oder aus(1 = ein, 0 = aus); Rückgabewert == 0 => Fehler

    string character_set_name();        // Gibt den Standardzeichensatz für die aktuelle Verbindung zurück.
    my_bool commit();                   // Committet die Transaktion. Was diese Funktion tut, hängt von dem Wert der Systemvariablen completion_type ab. Wenn der
                                        // completion_type 2 ist, gibt der Server nach Beendigung der Transaktion Ressourcen frei und schlieﬂt die Clientverbindung.
                                        // Das Clientprogramm sollte mysql_close() aufrufen, um die Verbindung von der Clientseite aus zu schlieﬂen.
    string error();
    int errnum();
		
    void debug(string debug); 				// Führt ein DBUG_PUSH mit dem angegebenen String durch
    int dump_debug_info();                  // Lässt den Server Debugginginformationen in das Log schreiben, benötigt Superrechte; Rückgabewert == 0 => OK!
    unsigned long real_escape_string( string to, const string from, unsigned long length); 		// Versieht Sonderzeichen in einem String, der in einer SQL-Anweisung benutzt
                                                                                                // werden soll, mit Escape-Symbolen
    unsigned int field_count(); 				// Liefert die Anzahl der Ergebnisspalten der letzten Anweisung
    void free_result();                         // Gibt den von einer Ergebnismenge belegten Speicher frei
    string get_client_info(); 					// Liefert Versionsinformationen f¸r den Client als String
    unsigned long get_client_version();			// Liefert einen Integer, der die Version der Clientbibliothek darstellt. Der Wert hat das Format XYYZZ, wobei X die
                                                // Hauptversion, YY der Release-Level und ZZ die Versionsnummer innerhalb des Releases ist. So bedeutet beispielsweise der
                                                // Wert 40102 die Clientbibliotheksversion 4.1.2.
    string get_host_info();                     // Liefert einen String, der die Verbindung beschreibt
    unsigned long get_server_version();         // Liefert die Versionsnummer des Servers als Integer.
    unsigned int get_proto_info();              // Liefert die Protokollversion der Verbindung
    string get_server_info();                   // Liefert die Versionsnummer des Servers
    string info();                              // Liefert Informationen ¸ber die zuletzt ausgef¸hrte Anfrage
    my_ulonglong insert_id();                   // Liefert die für eine AUTO_INCREMENT-Spalte von der vorhergehenden Anfrage generierte ID
    int kill(unsigned long pid); 				// Hält einen gegebenen Thread an, 0 bei Erfolg
    unsigned int num_fields();                  // Liefert die Anzahl der Spalten einer Ergebnismenge
    my_ulonglong num_rows();                    // Liefert die Anzahl der Zeilen einer Ergebnismenge
		
    int ping();                                 // Prüft, ob die Serververbindung funktioniert, und baut bei Bedarf eine neue Verbindung auf, 0 wenn OK
    int shutdown(enum mysql_enum_shutdown_level shutdown_level); 		// Fährt den Datenbankserver herunter;http://dev.mysql.com/doc/refman/5.1/de/mysql-shutdown.html;
    string stat();                              // Liefert den Serverstatus als String	; 0 bei Fehler
    unsigned long thread_id();                  // Liefert die aktuelle Thread-ID
    unsigned int warning_count();               // Liefert die Anzahl der Warnungen für die vorangehende SQL-Anweisung

    int refresh(unsigned int options); 			// Schreibt Tabellen und Caches auf die Platte zurück oder setzt sie zurück

    // Noch nicht implementierte Funktionen

    // list_dbs()                           // benutze SHOW DATABASES [LIKE wild]
    // list_fields()                        // SHOW COLUMNS FROM tbl_name anstelle von mysql_list_fields()
    // list_processes()                     // Liefert eine Liste der aktuellen Server-Threads ==> SHOW PROCESSLIST
    // list_tables()                        // Liefert Tabellennamen, die mit einem einfachen regulären Ausdruck übereinstimmen	==> SHOW tables [LIKE wild].
    // options()                            // Stellt Verbindungsoptionen für mysql_connect() ein
    // real_escape_string()					// Versieht Sonderzeichen in einem String, der in einer SQL-Anweisung benutzt werden soll, mit Escape-Symbolen, wobei der
                                            // aktuelle Zeichensatz der Verbindung berücksichtigt wird
    // reload();							// Lässt den Server die Berechtigungstabellen neu laden ==> FLUSH PRIVILEGES
    // change_user() 						// ändert den Benutzer und die Datenbank für eine offene Verbindung
};

#endif /* defined(__SLive__CDatabase_Connection__) */
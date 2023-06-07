#include <iostream>
#include <mysql.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>


class DatabaseConnection {
private:
    MYSQL* conn;

public:
    // Constructor
    DatabaseConnection(const char* host, const char* user, const char* pass, const char* db_name) {
        conn = mysql_init(NULL);
        if (!mysql_real_connect(conn, host, user, pass, db_name, 0, NULL, 0)) {
            std::cout << "Failed to connect to the database: " << mysql_error(conn) << std::endl;
            conn = nullptr; // Set the connection to nullptr to indicate a failure
        }
    }

    // Destructor
    ~DatabaseConnection() {
        if (conn != nullptr) {
            mysql_close(conn);
        }
    }

    // Function to execute a query and return the result set
    MYSQL_RES* executeQuery(const char* query) {
        if (conn == nullptr) {
            std::cout << "No database connection available." << std::endl;
            return nullptr;
        }

        if (mysql_query(conn, query)) {
            std::cout << "Query execution error: " << mysql_error(conn) << std::endl;
            return nullptr;
        }

        return mysql_use_result(conn);
    }

    // Function to free the result set
    void freeResultSet(MYSQL_RES* res) {
        if (res != nullptr) {
            mysql_free_result(res);
        }
    }
    
    MYSQL* getConnection() const {
        return conn;
    }
    
};

class People{
	protected:
		
		/*
		int id_endereco,id_people;
		//people variables
		char CPF[14], nome[256], email[256],telefone[15],caracterisca[256];
		
		//adress variables
		char logradouro[256], cep[10], numero[15], bairro[256];
		*/
		
	public:	
		
		//Creating a dynamic function...
		void create(DatabaseConnection& dbConnection, const char* tabela_nome, const char* coluna1, const char* coluna2, const char* coluna3, const char* coluna4, const char* coluna5, 
		const char* CPF, const char* nome, const char* email, const char* telefone, const char* caracterisca, const char* logradouro,const char* cep, const char* numero, const char* bairro){
			
			MYSQL_STMT* stmt = mysql_stmt_init(dbConnection.getConnection());
    
		    //Creating Endereco
		    
		    const char* insert_query_endereco = "INSERT INTO endereco (logradouro, cep, numero, bairro) VALUES (?, ?, ?, ?)";
		    mysql_stmt_prepare(stmt, insert_query_endereco, strlen(insert_query_endereco));
		
		    MYSQL_BIND endereco_info[4];
		    memset(endereco_info, 0, sizeof(endereco_info));
		    
		    endereco_info[0].buffer_type = MYSQL_TYPE_STRING;
		    endereco_info[0].buffer = (void*)logradouro;
		    endereco_info[0].buffer_length = strlen(logradouro);
		    
		    endereco_info[1].buffer_type = MYSQL_TYPE_STRING;
		    endereco_info[1].buffer = (void*)cep;
		    endereco_info[1].buffer_length = strlen(cep);
		    
		    endereco_info[2].buffer_type = MYSQL_TYPE_STRING;
		    endereco_info[2].buffer = (void*)numero;
		    endereco_info[2].buffer_length = strlen(numero);
			
		    endereco_info[3].buffer_type = MYSQL_TYPE_STRING;
		    endereco_info[3].buffer = (void*)bairro;
		    endereco_info[3].buffer_length = strlen(bairro);
		    
		    mysql_stmt_bind_param(stmt, endereco_info);
		    mysql_stmt_execute(stmt);
		
		    int lastInsertId = mysql_stmt_insert_id(stmt);
		    
		    mysql_stmt_close(stmt);
		
		    //Creating...
			
			stmt = mysql_stmt_init(dbConnection.getConnection());
			
			//Transforming the C type string in to c++ type of string to concatenate...
			std::string insert_query = "INSERT INTO " + std::string(tabela_nome) + "(" + std::string(coluna1) + "," + std::string(coluna2) + "," + std::string(coluna3) + "," + std::string(coluna4) + "," + std::string(coluna5) + ", id_endereco) VALUES (? , ?, ?, ?, ?, ?)";
			
			//Transforming the C++ type string in to c type of string because the "mysql_stmt_prepare" expect that...
			mysql_stmt_prepare(stmt, insert_query.c_str(), strlen(insert_query.c_str()));
			
			MYSQL_BIND data[6];
			memset(data, 0, sizeof(data));
		
			//info...
			data[0].buffer_type = MYSQL_TYPE_STRING;
			data[0].buffer = (void*)CPF;
			data[0].buffer_length = strlen(CPF);
		
			data[1].buffer_type = MYSQL_TYPE_STRING;
			data[1].buffer = (void*)nome;
			data[1].buffer_length = strlen(nome);
			    
			data[2].buffer_type = MYSQL_TYPE_STRING;
			data[2].buffer = (void*)email;
			data[2].buffer_length = strlen(email);
			    
			data[3].buffer_type = MYSQL_TYPE_STRING;
			data[3].buffer = (void*)telefone;
			data[3].buffer_length = strlen(telefone);
			
			data[4].buffer_type = MYSQL_TYPE_STRING;
			data[4].buffer = (void*)caracterisca;
			data[4].buffer_length = strlen(caracterisca);
			    
			// Convert lastInsertId to a string
			char lastInsertIdStr[20];
			sprintf(lastInsertIdStr, "%i", lastInsertId);
			    
			data[5].buffer_type = MYSQL_TYPE_STRING;
			data[5].buffer = lastInsertIdStr;
			data[5].buffer_length = strlen(lastInsertIdStr);
				
			mysql_stmt_bind_param(stmt, data);
			mysql_stmt_execute(stmt);
		
		    mysql_stmt_close(stmt);
    
		}
		
		void read_all(DatabaseConnection& dbConnection, const char* table,const char* caracterisca){ 
			char query[100];
		    
		    snprintf(query, sizeof(query), "SELECT * from %s as f, endereco as e WHERE e.id_endereco = f.id_endereco",table);
		
		    MYSQL_RES* res = dbConnection.executeQuery(query);
		    
		    if (res != nullptr) {
		        MYSQL_ROW row;
		        
		        while ((row = mysql_fetch_row(res)) != nullptr) {
		        	printf("\n-=Personal info from %s (%s) - ID (%s)=-",table,row[2],row[0]);
		            std::cout << "\nID: " << row[0] << ", CPF: " << row[1] << ", Nome: " << row[2] << ", Email: " << row[3] << ", Telefone: " << row[4] << ", " << std::string(caracterisca) << ": " << row[5] << "\n\nAdress info:\nID Endereco: " << row[7] << ", Lougradouro: " << row[8] << ", Cep: " << row[9] << ", Numero residencial: " << row[10] << ", Bairro: " << row[11] << std::endl;
				}
			}
			dbConnection.freeResultSet(res);
		}
		
		void read_all_id(DatabaseConnection& dbConnection, const char* table){
		    char query[100];
		    
		    snprintf(query, sizeof(query), "SELECT id_%s FROM %s",table,table);
		
		    MYSQL_RES* res = dbConnection.executeQuery(query);
		    
		    if (res != nullptr) {
		        MYSQL_ROW row;
		        
		        printf("\nID's from all %s:\n",table);
		        int count = 0;
		        while ((row = mysql_fetch_row(res)) != nullptr) {
		        	
					count = count+1;
		            std::cout << "ID: "<< row[0] << ",";
		            
		            if ((count % 5) == 0){
		            	printf("\n");
					}
		            
				}
			}
			
			dbConnection.freeResultSet(res);
		}
		
		void read_(DatabaseConnection& dbConnection, const char* tabela_nome , const char* caracterisca, int id){
		    char query[100];
		    snprintf(query, sizeof(query), "SELECT * from %s WHERE id_%s = %d", tabela_nome, tabela_nome, id);
		
		    MYSQL_RES* res = dbConnection.executeQuery(query);
		    
		    if (res != nullptr) {
		        MYSQL_ROW row;
		        
		        while ((row = mysql_fetch_row(res)) != nullptr) {
		        	
		        	printf("Personal info from %s (%s) - ID (%s)\n\n",tabela_nome,row[2],row[0]);
		        	
		            std::cout << "ID: " << row[0] << ", CPF: " << row[1] << ", Nome: " << row[2] << ", Email: " << row[3] << ", Telefone: " << row[4] << ", " << caracterisca << ": " << row[5] << std::endl;
		            
		            dbConnection.freeResultSet(res);
		            
		            snprintf(query, sizeof(query), "SELECT * FROM endereco WHERE id_endereco = %s", row[6]);
		        	MYSQL_RES* res = dbConnection.executeQuery(query);
		        	
				    if (res != nullptr) {
				        MYSQL_ROW adress_info;
				        
				        while ((adress_info = mysql_fetch_row(res)) != nullptr) {
				        	
				        	printf("\nAdress info from %s\n\n",tabela_nome);
				        	
				            std::cout << "Logradouro: " << adress_info[1] << ", CEP: " << adress_info[2] << ", Numero: " << adress_info[3] << ", Bairro: " << adress_info[4] << std::endl;
				        }
				
				        dbConnection.freeResultSet(res);
				    }
		        
				}
		
		    }
		}
		
		void update_(DatabaseConnection& dbConnection, const char* tabela_nome, const char* coluna1,int id, const char* CPF, const char* nome, const char* email, const char* telefone, const char* logradouro, const char* caracterisca, const char* cep, const char* numero, const char* bairro){
			MYSQL_STMT* stmt = mysql_stmt_init(dbConnection.getConnection());
			
			//Selecting adress id
		    
		    char query[100];
		    snprintf(query, sizeof(query), "SELECT * FROM %s WHERE id_%s = %d",tabela_nome ,tabela_nome , id);
		
		    MYSQL_RES* res = dbConnection.executeQuery(query);
			
		    MYSQL_ROW row;
		    row = mysql_fetch_row(res);
		    
		    int id_address = atoi(row[6]);
		    mysql_free_result(res);
			
			//Updating...
		    std::string update_query = "UPDATE " + std::string(tabela_nome) + " SET CPF = ?, nome = ?, email = ?, telefone = ?, " + std::string(coluna1) + " = ? WHERE id_" + std::string(tabela_nome) + " = ?";
		    mysql_stmt_prepare(stmt, update_query.c_str(), strlen(update_query.c_str()));
		
		    MYSQL_BIND bind[6];
		    memset(bind, 0, sizeof(bind));
		
		    bind[0].buffer_type = MYSQL_TYPE_STRING;
		    bind[0].buffer = (void*)CPF;
		    bind[0].buffer_length = strlen(CPF);
		    
		    bind[1].buffer_type = MYSQL_TYPE_STRING;
		    bind[1].buffer = (void*)nome;
		    bind[1].buffer_length = strlen(nome);
		    
		    bind[2].buffer_type = MYSQL_TYPE_STRING;
		    bind[2].buffer = (void*)email;
		    bind[2].buffer_length = strlen(email);
		    
		    bind[3].buffer_type = MYSQL_TYPE_STRING;
		    bind[3].buffer = (void*)telefone;
		    bind[3].buffer_length = strlen(telefone);
		    
		    bind[4].buffer_type = MYSQL_TYPE_STRING;
		    bind[4].buffer = (void*)caracterisca;
		    bind[4].buffer_length = strlen(caracterisca);
		    
		    bind[5].buffer_type = MYSQL_TYPE_LONG;
		    bind[5].buffer = (void*)&id;
		    
		    mysql_stmt_bind_param(stmt, bind);
		    mysql_stmt_execute(stmt);
		        
			//Updating adress
			
		    const char* update_query_adress = "UPDATE endereco SET logradouro = ?, cep = ?, numero = ?, bairro = ? WHERE id_endereco = ?";
		    mysql_stmt_prepare(stmt, update_query_adress, strlen(update_query_adress));
		
		    MYSQL_BIND bind_adress[5];
		    memset(bind_adress, 0, sizeof(bind_adress));
		
		    bind_adress[0].buffer_type = MYSQL_TYPE_STRING;
		    bind_adress[0].buffer = (void*)logradouro;
		    bind_adress[0].buffer_length = strlen(logradouro);
		    
		    bind_adress[1].buffer_type = MYSQL_TYPE_STRING;
		    bind_adress[1].buffer = (void*)cep;
		    bind_adress[1].buffer_length = strlen(cep);
		    
		    bind_adress[2].buffer_type = MYSQL_TYPE_STRING;
		    bind_adress[2].buffer = (void*)numero;
		    bind_adress[2].buffer_length = strlen(numero);
		    
		    bind_adress[3].buffer_type = MYSQL_TYPE_STRING;
		    bind_adress[3].buffer = (void*)bairro;
		    bind_adress[3].buffer_length = strlen(bairro);
		    
		    bind_adress[4].buffer_type = MYSQL_TYPE_LONG;
		    bind_adress[4].buffer = (void*)&id_address;
		    
		    mysql_stmt_bind_param(stmt, bind_adress);
		    mysql_stmt_execute(stmt);
		    
		    mysql_stmt_close(stmt);
			
		}
		
		void delete_(DatabaseConnection& dbConnection,const char* tabela_nome ,int id){
			char query[100];
		    
		    //Getting adress ID
		    snprintf(query, sizeof(query), "SELECT * FROM %s WHERE id_%s = %d",tabela_nome,tabela_nome,id);
		
		    MYSQL_RES* res = dbConnection.executeQuery(query);
		    
		    MYSQL_ROW row;
		    row = mysql_fetch_row(res);
		    
		    int id_address = atoi(row[6]);
		    
		    // Free the result set before executing additional queries
		    dbConnection.freeResultSet(res);
		    
		    //Del funcionario
		    snprintf(query, sizeof(query), "DELETE FROM %s WHERE id_%s = %i",tabela_nome,tabela_nome,id);
		
		    res = dbConnection.executeQuery(query);
		    
			//Del adress
			snprintf(query, sizeof(query), "DELETE FROM endereco WHERE id_endereco = %i", id_address);
			
		    res = dbConnection.executeQuery(query); 
		}
};

/*
class Funcionario : public People{		
	public:	
		
};


class Medico : public People{		
	public:	
		
};


class Paciente : public People{		
	public:	
		
};
*/

int main() {
    // Colocando tipos de caracter para UTF-8
    setlocale(LC_ALL, "portuguese");

    const char* HOST = "localhost";
    const char* USER = "root";
    const char* PASS = "root";
    const char* DB_NAME = "hospital_v2";

    DatabaseConnection dbConnection(HOST, USER, PASS, DB_NAME);
	
	//Criando instancia do funcionario
	People instancia_funcionario;
	
	
	/////////////////////
	//Testando metodos//
	///////////////////
	
	//instancia_funcionario.create(dbConnection,"funcionario","CPF","nome","email","telefone","funcao","123.123.123-45","Gustavo Leandro Gorges","gustavo.gorges@faculdadecesusc.edu.br","4832695585","Desenvolvedor Junior","Rua evaristo guilherme dos santos","14785-569","130","Vargem de fora");
	//instancia_funcionario.read_all(dbConnection,"funcionario","funcao");
	//instancia_funcionario.read_all_id(dbConnection,"funcionario");
	//instancia_funcionario.read_(dbConnection,"funcionario","funcao",40);
	//instancia_funcionario.update_(dbConnection,"funcionario","funcao",40,"657.657.657-78","new name","newemail@gmail.com","4899999999","newlogradouro","Engenheiro de software","45673-677","122","Ingleses do rio vermelho");
	//instancia_funcionario.delete_(dbConnection,"funcionario",42);
	
	std::string cpf_create,nome_create,email_create,telefone_create,logradouro_endereco,funcao_create,cep_endereco,numero_endereco,bairro_endereco;
	int ids,ids_create,id_hospital; 
	
	char menu,start;
	
	start:
	
	//Starting the main menu
	do{
		system("cls");
		
		printf("Hospital - Menu\nInsert the desired table:\n\n");
		printf("1 - Funcionario\n2 - Medico\n3 - Hospital\n4 - Paciente\n5 - Clinica\n6 - Doencas\n7 - Prontuario\n\nType 8 to exit.");
		menu = getche();
	}while(menu > '8' || menu < '1');
	
	switch (menu){
		
		//funcionario
		case '1':{
				do{
					system("cls");
					printf("Funcionario - Menu\nWhat do you want to do?:\n");
					printf("1 - Create\n2 - Read\n3 - Update\n4 - Delete\n\nType 5 to return.");
					menu = getche();
					
					switch(menu){
						
						//Create
						case '1':{
							system("cls");
							
							std::cout << "Personal INFO:" << std::endl;
							std::cout << "Insert the CPF: " << std::endl;
							std::getline(std::cin, cpf_create);
							
							std::cout << "Insert the nome: " << std::endl;
							std::getline(std::cin, nome_create);
							
							std::cout << "Insert the email: " << std::endl;
							std::getline(std::cin, email_create);
							
							std::cout << "insert the telefone: " << std::endl;
							std::getline(std::cin, telefone_create);
							
							std::cout << "insert the cargo: " << std::endl;
							std::getline(std::cin, funcao_create);
							
							std::cout << "\nAdress INFO:\n" << std::endl;
							
							std::cout << "insert the logradouro: " << std::endl;
							std::getline(std::cin, logradouro_endereco);
							
							std::cout << "insert the CEP: " << std::endl;
							std::getline(std::cin, cep_endereco);
							
							std::cout << "insert the numero residencial: " << std::endl;
							std::getline(std::cin, numero_endereco);
							
							std::cout << "insert the bairro: " << std::endl;
							std::getline(std::cin, bairro_endereco);
							
							//I needed to tranform all the string values to char values...
							instancia_funcionario.create(dbConnection,"funcionario","CPF","nome","email","telefone","funcao",cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
							
							goto start;
							break;
						}
						
						//Read
						case '2':{
							do{
							system("cls");
							printf("Funcionario - Read\nWhat do you want to do?:\n\n");
							printf("1 - Read all\n2 - Read by ID\n\nType 3 to return.");
							menu = getche();
							
							}while(menu > '3' || menu < '1');
							
							switch(menu){
								//READ ALL
								case '1':{
									printf("\n\nALL DATAS FROM FUNCIONARIO:\n\n");
									instancia_funcionario.read_all(dbConnection,"funcionario","funcao");
									printf("\n");
									
									system("pause");
									goto start;
									break;
								}
								
								//READ BY ID
								case '2':{
									system("cls");
									std::cout << "0 to leave." << std::endl;
										
									instancia_funcionario.read_all_id(dbConnection,"funcionario");
									
									do{	
										printf("\n-=-=-=-=*-*-=-=-=-=\n");
										std::cout << "\ninsert the id: " << std::endl;
										std::cin >> ids;
										
										instancia_funcionario.read_(dbConnection,"funcionario","funcao",ids);
									
									}while(ids != 0);
									
									goto start;
									break;
								}
								
								//Return
								case '3':{
									goto start;
									break;
								}
							}
							
							break;
						}
						
						//Update
						case '3':{
							system("cls");
							
							std::cout << "Personal INFO:" << std::endl;
							
							instancia_funcionario.read_all_id(dbConnection,"funcionario");
							
							printf("\n");
							
							std::cout << "Insert the ID: " << std::endl;
							std::cin >> ids_create;
							
							std::cin.ignore();  // Discard the remaining newline character
							
							std::cout << "Insert the CPF: " << std::endl;
							std::getline(std::cin, cpf_create);
							
							std::cout << "Insert the nome: " << std::endl;
							std::getline(std::cin, nome_create);
							
							std::cout << "Insert the email: " << std::endl;
							std::getline(std::cin, email_create);
							
							std::cout << "insert the telefone: " << std::endl;
							std::getline(std::cin, telefone_create);
							
							std::cout << "insert the cargo: " << std::endl;
							std::getline(std::cin, funcao_create);
							
							std::cout << "\nAdress INFO:\n" << std::endl;
							
							std::cout << "insert the logradouro: " << std::endl;
							std::getline(std::cin, logradouro_endereco);
							
							std::cout << "insert the CEP: " << std::endl;
							std::getline(std::cin, cep_endereco);
							
							std::cout << "insert the numero residencial: " << std::endl;
							std::getline(std::cin, numero_endereco);
							
							std::cout << "insert the bairro: " << std::endl;
							std::getline(std::cin, bairro_endereco);
							
							//I needed to tranform all the string values to char values...
							instancia_funcionario.update_(dbConnection,"funcionario","funcao",ids_create,cpf_create.c_str(),nome_create.c_str(),email_create.c_str(),telefone_create.c_str(),logradouro_endereco.c_str(),funcao_create.c_str(),cep_endereco.c_str(),numero_endereco.c_str(),bairro_endereco.c_str());
							
							goto start;
							break;
						}
						
						//Delete
						case '4':{
							system("cls");
							printf("0 to return\n");
							instancia_funcionario.read_all_id(dbConnection,"funcionario");
							
							std::cout << "\nInsert the ID: " << std::endl;
							std::cin >> ids_create;
							
							if(ids_create == 0){
								goto start;
							}
							
							instancia_funcionario.delete_(dbConnection,"funcionario",ids_create);
							
							goto start;
							break;
						}
						
						//return
						case '5':{
							goto start;
							break;
						}
					}
					
					break;
				}while(menu > '5' || menu < '1');
			}
		
		//Medico
		case '2':{
			goto start;
			break;
		}
		
		//Hospital
		case '3':{
			goto start;
			break;
		}
		//Paciente
		case '4':{
			goto start;
			break;
		}
		//Clinica
		case '5':{
			goto start;
			break;
		}
		//Doencas
		case '6':{
			goto start;
			break;
		}
		//Prontuario
		case '7':{
			goto start;
			break;
		}
		//exit
		case '8':{
			printf("\n\n\nExiting...");
			break;
		}
		
	}
	
	dbConnection.~DatabaseConnection();
	
	return 0;
}

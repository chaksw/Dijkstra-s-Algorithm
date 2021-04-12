#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include "../header/Application_RATP.hpp"
using namespace std;
using namespace travel;


//Sucharger la fonction Gneric_stations_parser::read_stations
void Application_RATP::read_stations(const string& _filename)
{
	int stations_id;
	ifstream s(_filename.c_str());		//lire les data du d'un document
	if(s.fail())
	   cerr<<"entreerreur: impossible de lire le fichier"<<std::endl;
	else{
		Station t;
		string line;
		getline(s,line);
		//std::cout<<s.is_open()<<std::endl;
		while(getline(s,line))	//ligne par ligne
		{
			vector<string> infors;
			istringstream sin(line);	//lire le chaine de caractère et le mettre dans le flux 'sin'
			string infor;
			while(getline(sin,infor,','))	//mettre 'sin' dans 'infor'
			{
				infors.push_back(infor);	//mettre 'infor' dans 'infors'
			}
			//répartir les information
			//NOM
			t.name = infors[0];
			//ID de station
			stations_id = atoi(infors[1].c_str());
			//ID de ligne
			t.line_id = infors[2];
			//address
			t.address = infors[3];
			//nom de ligne
			t.line_name = infors[4];
			
			
			stations_hashmap[stations_id] = t ;	//Conversation string à uint64_t et les mettre dans stations_hashmap
			std::cout<<"------nom de station----"<<t.name<<std::endl;
		}
	s.close();	
	}
	
	//ergodique
	std::cout<<std::endl;
	std::cout<<"----------------------"<<"Stations_Hashmap"<<"---------------------------"<<std::endl;

	for(auto& x:stations_hashmap)
		std::cout<<"ID de staion: "<<x.first<<"\t"<<x.second<<std::endl;
	std::cout<<std::endl;
	
}


////Implémenter la fonction read_connections
void Application_RATP::read_connections(const string& _filename)
{
	
	ifstream s(_filename);		//lire les data du d'un document
	if(s.fail())
	   cerr<<"entre erreur: impossible de lire le fichier"<<std::endl;
	else
	{
		string line;
		getline(s,line);
		//std::cout<<s.is_open()<<std::endl;
		while(getline(s,line))		//ligne par ligne
		{
			vector<string> infors;
			istringstream sin(line);	//lire le chaine de caractère et le mettre dans le flux 'sin'
			string infor;
			while(getline(sin,infor,','))	//mettre 'sin' dans 'infor'
			{
				infors.push_back(infor);	//mettre 'infor' dans 'infors'
			}
			//répartir les information
			string id_depart = infors[0];	//id de départ
			string id_destin = infors[1];	//id de destination
			string temp = infors[2];		//temp nécessaire
		
			connections_hashmap[stoull(id_depart)][stoull(id_destin)]= stoull(temp);		

		}
	s.close();
	}

	
	//ergodique
	std::cout<<std::endl;
	std::cout<<"----------------------"<<"Connections_Hashmap"<<"---------------------------"<<std::endl;
	unordered_map<uint64_t,unordered_map<uint64_t,uint64_t> >::iterator it;
	for(it=connections_hashmap.begin();it != connections_hashmap.end();it++)
	{
		std::cout<<"Le départ: "<<it->first<<"\t";
		unordered_map<uint64_t,uint64_t> :: iterator it_2;
		for(it_2 = it->second.begin(); it_2 != it->second.end(); it_2 ++)
		{
			std::cout<<"La destination: "<<it_2->first<<"\tLe temp nécessaire: "<<it_2->second<<std::endl;
		}
	}
	std::cout<<std::endl;
	
}

//Implémenter la fonction compute_travel
vector<pair<uint64_t,uint64_t> > Application_RATP::compute_travel(uint64_t _start, uint64_t _end)
{
	vector<pair<uint64_t,uint64_t> > compute_travel;

	uint64_t tm = 999999;
	
	unordered_map<uint64_t,uint64_t> _prev;		//le noued précedent de l'ième noued
	unordered_map<uint64_t,uint64_t> _temp;		//la destination et le temp necessaire entre _start et le ième noeud
	unordered_map<uint64_t,uint64_t> isInS;		//vérifier si on a passé un noeud
	uint64_t n_courant;		//  noued courant
	
	
	unordered_map<uint64_t,unordered_map<uint64_t,uint64_t> >::iterator it;
	for(it=connections_hashmap.begin();it != connections_hashmap.end();it++)
	{		  
		unordered_map<uint64_t,uint64_t> :: iterator it_2;
		for(it_2 = it->second.begin(); it_2 != it->second.end(); it_2 ++)
		{
			_temp[it_2->first] = connections_hashmap[_start][it_2->first];
			isInS[it_2->first] = 0;	//on suppose que tous les noued n'est pas passés
			if(_temp[it_2->first]  > 0)	
				_prev[it_2->first] = _start;		
			else
				_prev[it_2->first] = 0;		//on ne sait pas encore le noeud précedent

		}
	}
	
	_prev[_start] = 0;		//n'a pas de noeud précedent pour le _start
	
	//-------------traitement des noeuds qui n'est pas encore passé par l'idéologie rapiate-------------//
	
	isInS[_start] = 1;	// premier noeud passé
	n_courant= _start;		//  noued courant
	//std::cout<<"----------------------------------------------------"<<std::endl;
	unordered_map<uint64_t,uint64_t> :: iterator it_t;
	for(it_t = _temp.begin(); it_t != _temp.end(); it_t++)
	{
		
		uint64_t n_suivant = n_courant;		
		uint64_t temp_min = tm;
		
		//trouver le temp minimal et le noeud correspondant qui va être un noued suivant
		for(auto& i: _temp)
		{	
			if((isInS[i.first]) == 0 && (_temp[i.first] != 0) && (_temp[i.first]< temp_min))
			{
				temp_min = _temp[i.first];	//le temp minimal
				n_suivant = i.first;	//le noeud correspondant qui va être un noued suivant
			}
		}
		
		isInS[n_suivant] = 1;
		//mise à jour isInS
			//si un noued suics les duex conditionos dessus, on les prend comme un noeud passé			
		n_courant= n_suivant;	//on étudie maintenant le noeud qui correspond au temp minimal
		
		//mise à jour le _temp et le _prev pour trouver les noeuds suivants
		for(auto& k:_temp)
		{
			if((isInS[k.first] == 0) && (connections_hashmap[n_courant][k.first]> 0) )
			{
				uint64_t temp = _temp[n_courant] + connections_hashmap[n_courant][k.first];
				
				if(temp < tm)
				{
					_temp[k.first] = temp;
					_prev[k.first] = n_courant;
				}
			}
		}
		if(_temp[_end] != 0) 	break;		//Si on trouve le temp monimal pour la destination
	
	}
	
	//--------------------------------fin de traitement des noeuds --------------------------------//
	
	// mettre tous les stations passées dans le vector compute_travel
	n_courant=_end;

	while(_prev[n_courant] != 0 )
	{
		compute_travel.push_back(make_pair(n_courant,_temp[n_courant]));
		n_courant = _prev[n_courant];
	}
	compute_travel.push_back(make_pair(_start,_temp[_start]));

	
	for(auto& i: compute_travel)
	{
		std::cout<<stations_hashmap[i.first]<<"---------------"<<i.second<<std::endl;
	}

	return compute_travel;
	
}

		
				

//Implémenter la fonction compute_and_display_travel
vector<pair<uint64_t,uint64_t> > Application_RATP::compute_and_display_travel(uint64_t _start, uint64_t _end)
{
	std::cout<<"-----------------on commence à chercher le temp minimal pour la destination--------------------"<<std::endl;
	std::cout<<std::endl;
	vector<pair<uint64_t,uint64_t> > ct;
	//Vérifier si exicite la station qu'on veut prendre
	if(connections_hashmap.find(_start) == connections_hashmap.end())
	{
		std::cout<<"Désolé, le départ que vous cherchez n'existe pas, veuillez vérifier si vous avez saisi correctement."<<std::endl;		//station non trouvé
	}else if(connections_hashmap.find(_end) == connections_hashmap.end())
	{
		std::cout<<"Désolé, la destination que vous cherchez n'existe pas, veuillez vérifier si vous avez saisi correctement."<<std::endl;
	}else{
		
		ct = compute_travel(_start,_end);
		std::cout<<"By the way from "<<stations_hashmap[_start]<<" to "<<stations_hashmap[_end]<<" is :"<<std::endl;
		vector<pair<uint64_t,uint64_t> > :: reverse_iterator it;
		for(it = ct.rbegin(); it != ct.rend(); it++)
		{
			if(it->first != _start)
			{
				if(stations_hashmap[it->first].line_id != stations_hashmap[(it+1)->first].line_id)
				{
					
					std::cout<<"Walk to ";
					std::cout<<stations_hashmap[it->first].name<<", line "<<stations_hashmap[it->first].line_id<<" ("<<it->second<<" secs)"<<std::endl;

					if(connections_hashmap[it->first][(it+1)->first] != 0)
					{
						std::cout<<"Take line"<<stations_hashmap[it->first].line_id<<" "<<stations_hashmap[it->first].line_name<<std::endl;
						std::cout<<"From "<<stations_hashmap[it->first]<<" to "<<stations_hashmap[(it+1)->first];
						std::cout<<"("<<connections_hashmap[it->first][(it+1)->first]<<" secs)"<<std::endl;
					}
				}

			}
		}
		}
	std::cout<<std::endl;
	std::cout<<"After "<<ct.front().second<<" secs,"<<"you have reached your destination!"<<std::endl;
	
	return	ct;
}
	

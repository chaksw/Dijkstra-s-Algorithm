#include <iostream>
#include "stdio.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <map>
#include "../header/appRatp.hpp"

using namespace std;
using namespace travel;

/**
 * @input: stations file
 * @function: read the flux of stations file and save in stations_hashmap
 * @output: A connection_hashmap, each element of hashmap containe a stations ID as key value and station's informations as value
 */
void appRatp::read_stations(const string& _filename)
{
    /**
     *  使用 ifstream 进行读取文件时，在其对应stream类中的构造函数中隐式得调取了open函数
     * 而open函数的形参需要是字符指针类型，由于_filename为string类型，所以在ifstream函
     * 数使用时需要使用c_str()函数，将字符串转化为字符指针。
     * */
    ifstream is(_filename,ios::in);
    if(!is.is_open())
    {
        cout << " error, cant not open file" << endl;
        return;
    }
    uint64_t station_id = 0;
    string line;
    getline(is,line);   // skip the frist line
    // cout << line << endl;
    while (getline(is,line))
    {
        /* 从文件流中获取一行数据存储在line中，getline函数包含在string库中 */
        
        /* 以行为单位，将每行数据对应的数据属性存储到Station结构体中 */
        istringstream sin(line);    // istringstream 可以用于分割被空格、制表符等符号分割的字符串
        Station s;    
        vector<string> infors;
        string infor;
        while (getline(sin,infor,','))
        {
            infors.push_back(infor);
        }
        s.name = infors[0];         // station name
        station_id = atoi(infors[1].c_str());
        s.line_id = infors[2];      // line ID
        s.address = infors[3];      // station address
        s.line_name = infors[4];    // line name
        stations_hashmap[station_id] = s;
    }
    is.close();
    // unordered_map
    // cout << "<--------------------Stations Informations-------------------->" << endl;
    // for(auto& iter:stations_hashmap)
    // {
    //     cout << "Station ID : " << iter.first << "\t" << iter.second << endl;
    // }
    //// 同效果
    // cout << endl;
    // for(auto iter = stations_hashmap.begin(); iter != stations_hashmap.end(); iter++)
    // {
    //     cout << "Station ID : " << iter->first << "\t" << iter->second << endl;
    // }
}

/**
 * @input: connections file
 * @function: read the flux of connection file and save in connections_hashmap
 * @output: A connection_hashmap, each element of hashmap containe a start stop, the end stops accessible as 
 *          key value and the time nessarrily as value
 */
void appRatp::read_connections(const string& _filename)
{
    ifstream is(_filename,ios::in);
    if(!is.is_open())
    {
        cout << " error, cant not open file" << endl;
        return;
    }
    string line;  
    getline(is,line);   // skip the frist line
    while (getline(is,line))
    {
        /* 从文件流中获取一行数据存储在line中，getline函数包含在string库中 */
        // getline(is,line);
        /* 以行为单位，将每行数据对应的数据属性存储到Station结构体中 */
        istringstream sin(line);    // istringstream 可以用于分割被空格、制表符等符号分割的字符串
        vector<string> infors;
        string infor;
        /** 
         * 不要使用eof函数
         * eof()返回true的条件是“读到文件结束符”，而不是文件内容的最后一个字符。
         * “文件结束符”不是指文件最后的字符，而是文件最后的字符的下一位。
         * 在读到为文件尾后，编译器会让指针停留在最后一个字符那里，然后重复读取一次。
         * 这样在该项目下会导致访问了不存在的地址，即段错误。
         * */
        while (getline(sin,infor,','))
        {
            infors.push_back(infor);
        }
        /**
         *  c_str() 会创建一个临时的string对象，这个对象的存活周期和使用它的表达式一样长，
         *  所以在下面这条命令结束后，infors[0].c_str()所创建的对象会消失，导致atoi(infors[0].c_str())][atoi(infors[1].c_str())]
         *  中atoi()）内部的指针为悬空，导致段错误（访问了不存在的地址）。
         *  正确做法是建立一个与函数生命周期一样长的临时变量存储infors[0].c_str()的内容。
         * */
        string from_id = infors[0];	//id de départ
        string to_id = infors[1];	//id de destination
        string time = infors[2];		//time nécessaire
		
        connections_hashmap[stoull(from_id)][stoull(to_id)]= stoull(time);
    }
    is.close();
    // unordered_map
    // cout << "<--------------------Connections Informations-------------------->" << endl;
    // for(auto& iter1:connections_hashmap )
    // {
    //     cout << "From stop " << iter1.first << " to stop: " << endl;
    //     for(auto &iter2 : iter1.second)
    //     {
    //         cout << "\t\t\t" << iter2.first << "\t need " << iter2.second << " mins" << endl;
    //     }
    // }
}

/**
 * @input: start station ID & end station ID
 * @function: Dijkstar's Algorthim
 * @output: The set of vertices of the shortest path
 */
vector<pair<uint64_t,uint64_t>> appRatp::compute_travel(uint64_t _start, uint64_t _end)
{
    /** 初始化：
     * 1. 建立最短路径集合 S 。
     * 2. 找到与起点相邻的顶点，记录时间（距离）。
     */
    // 1. 建立最短路径集合。
    unordered_map<uint64_t,uint64_t> S;         // 最短路径顶点集合
    // vector<pair<uint64_t,uint64_t>> S;          
    unordered_map<uint64_t,uint64_t> isVisited; // 访问flag, 避免对顶点的重复访问
    unordered_map<uint64_t,uint64_t> _prev;     // 记录最短路径上每个顶点的前一个顶点


    S[_start] = 0;           // 先插入起点
    // 初始化起点属性
    isVisited[_start] = 1;                      // 1->visited, 2-> not visited
    _prev[_start] = 0;
    uint64_t t_inf = 999999;

    // 2. 找到与起点相邻的顶点，记录时间（距离）。
    unordered_map<uint64_t,uint64_t> times; 
    unordered_map<uint64_t, unordered_map<uint64_t,uint64_t>>::iterator iter1;


    for(iter1 = connections_hashmap.begin(); iter1 != connections_hashmap.end(); iter1++)
    {
        for(auto &iter2:iter1->second)
        {
            if(iter1->first == _start)
            {       
                _prev[iter2.first] = _start;
                times[iter2.first] = iter2.second;
            }else if(times.find(iter2.first) == times.end())    // 避免赋值错误
            {
                times[iter2.first] = t_inf;
            }
            isVisited[iter2.first] = 0;
        }
        
    }

    /**
     * 1. 找到距离起点距离最短的顶点，并放入集合S中
     * 2. 根据新的集合S找到其他与起点有可达关系的顶点，并更新距离值。
     */
    // cout << "<--------------- get the min path set S --------------->" << endl;
    uint64_t id_min = 0;
    while(1)
    {
        // 1. 找到距离起点距离最短的顶点，并放入集合S中
        uint64_t t_min = t_inf;
        for(auto& iter_t:times)
        {
            if((isVisited[iter_t.first] == 0) && (iter_t.second < t_min))
            {
                id_min = iter_t.first;
                t_min = iter_t.second;
                // t_min = iter_t.second + times[_prev[iter_t.first]];
                
            }
        }
        S[id_min] = t_min;
        // S.push_back(make_pair(id_min,(t_min+times[_prev[id_min]])));
        isVisited[id_min] = 1;  // 更新对应的访问flag
        if(id_min == _end)
        {
            break;  // 找到终点即停止循环
        }
        // 2. 根据新的集合S找到其他与起点有可达关系的顶点，并更新距离值。
        if ((iter1 = connections_hashmap.find(id_min)) != connections_hashmap.end())
        {
            for(auto &iter2:iter1->second)
            {
                // if((iter2.second > 0) && ((iter2.second + times[id_min]) < times[iter2.first]))
                if((iter2.second + times[id_min]) < times[iter2.first])
                {
                    times[iter2.first] = iter2.second + times[id_min];
                    _prev[iter2.first] = id_min;
                }   
            }
        }  
    }

    /**
     * 上述迭代后，可以得到以 _start 到_end的最短距离。
     * 接下来需要结合_prev 和 S集合将具体路径放入输出中。
     */
    // cout << "<--------------- get the min path --------------->" << endl;
    vector<pair<uint64_t,uint64_t>> min_path;
    uint64_t path_id = _end;
    while(1)
    {
        min_path.push_back(make_pair(path_id,S[path_id]));
        path_id = _prev[path_id];
        if (path_id == _start)
        {
            min_path.push_back(make_pair(_start,S[_start])); 
            break;
        }
    }
    return min_path;
}

/**
 * @input: start station ID & end station ID
 * @function: Print the shortest path in terminal
 * @output: The set of vertices of the shortest path
 */
vector<std::pair<uint64_t,uint64_t>> appRatp:: compute_and_display_travel(uint64_t _start, uint64_t _end)
{
    // 先判断要寻找的路线起点和终点是否合法
    vector<pair<uint64_t,uint64_t>> path;
    if(stations_hashmap.find(_start) == stations_hashmap.end())
    {
        cout << "No start station not found, please try agin." << endl;
        cout << "======================================" << endl;
        return path;
    }else if (stations_hashmap.find(_end) == stations_hashmap.end())
    {
        cout << "No end station not found, please try agin." << endl;
        cout << "======================================" << endl;
        return path;
    }
    path = compute_travel(_start,_end);
    uint64_t hour = path[0].second/60;
    uint64_t min = path[0].second % 60;
    cout << "From station " << _start << "->" << stations_hashmap[_start].name;
    cout << " To station " << _end << "->" << stations_hashmap[_end].name << endl;
    cout << "Required time: "<< hour << "h" << min << "min" << " (" << path[0].second << " mins" << ")" << endl;

    cout << endl;
    cout << "<--------------------------- Specific route --------------------------->" << endl;
    cout << endl;

    string line_id = stations_hashmap[_start].line_id;
    cout << "Start station: " << stations_hashmap[_start].name << endl;
    cout << "Take ligne " << stations_hashmap[_start].line_id << " in " << stations_hashmap[_start].address << endl;
    vector<pair<uint64_t,uint64_t>>::reverse_iterator iter_path;
    for (iter_path = path.rbegin(); iter_path != path.rend(); iter_path++)
    {
        if(iter_path->first == _start)
        {
            iter_path++;
        }
        if(iter_path->first != _end){
            if((stations_hashmap[iter_path->first].line_id) == line_id)
            {
                cout << "\t | \t" << stations_hashmap[iter_path->first].name << endl; 
                // continue;
            }else{
                line_id = stations_hashmap[iter_path->first].line_id;
                cout << "At station " << stations_hashmap[iter_path->first].name;
                cout << "\tinterchange ligne " << line_id << " in " << stations_hashmap[iter_path->first].address << endl;
                // continue;
            }
        }
    }
    cout << "End station: " << stations_hashmap[_end].name << endl;
    return path;
        
}

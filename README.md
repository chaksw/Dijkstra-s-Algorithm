# 综述
一次C++知识的应用，基于对类的封装（public, protected, private关键字）、多态（虚函数的重写- 晚绑定），结合文件I/O 和 [[地铁最短路径计算[[STL]]容器特性|STL容器特性]]，使用合适的容器，通过[[戴克特斯拉算法 (Dijkstra's Algorithm)|Dijkstar‘s algorithm]] 实现任意两点最短路径的计算。
## 文件的读取
- 文件为csv格式，每列的数据是以 , 隔开的，可以使用getline函数定义字符串的分隔提取条件为 ',' ，
## 容器的使用
使用vector数组存储分隔后的字符串，并逐个放入unordered_map容器当中。
### 使用vector容器的原因
与vector相对的，我们还有list 和 queue，项目中需要一个临时的数据结构来存放分隔后的信息，并随后访问该数据结构将对应的字符串存放入结构体数组中。我们**对于临时存放没有顺序要求，也不需要头插和中间插入需求，但需要进行数据访问操作，而vector在三者中的数据访问速度是最快的。**
### unordered_map 的使用
由于[[戴克特斯拉算法 (Dijkstra's Algorithm)]] 的使用需要大量对数据对查找操作，相比map，**unordered_map 的查找问题上更加高效**。另外，地铁线路图可以理解为[[有向图 (Directed Graphs)#邻接表|邻接表]]的数据形式（不考虑数据的顺序，但又希望高的查找效率），为此我们可以通过unordered_map的hash table底层逻辑建立邻接表。

 ## STL容器特性
### vector 容器
vector的底层可以理解为动态数组，它可以初始化一个连续的动态存储空间，因而访问效率在三者之间是最高的。（即支持 $[ \ ]$ 操作符和vector.at()）在空间不足时自动分配更大的空间（但这个分配过程比较耗时）。基于其为动态数组，其优点和缺点如下：
#### 优缺点
- 可以动态的分配空间，当动态添加的数据超过vector默认分配的大小时要进行整体的重新分配、拷贝与释放。
- 继承了数组的特点，因其为连续存储空间，故而访问非常方便且快速，即支持 $[ \ ]$ 操作符和vector.at( )，但相对的，在内部进行插入删除操作效率低，且智能在尾部进行push和pop
### list 容器
list容器可以理解为一个动态双向链表，故而可以进行头插和尾插，基于链表的特性，其分配的存储空间不需要是连续的。
#### 优缺点
- list 和 vector的优缺点基本是相反的（也就是动态数组和动态链表的区别），由于list的存储空间不是连续的，因而内部的插入和删除都十分方便，但是其不支持 $[ \ ]$ 操作符和.at( )对数据进行访问，每次访问数据都需要对整个数据组进行遍历。
- 由于其为动态双向链表，所以不需要考虑默认存储空间不足的问题，且支持头部和尾部的push和pop
- 相比 vetor ，list 占用的内存比较多。
### deque 容器
deque 容器在功能上合并了vector 和 list 功能，它是一个双向队列，故而支持 $[ \ ]$ 操作符和.at( )对数据进行访问，也支持头部和尾部的push和pop。内部的插入和删除都比较方便。
#### 优缺点
其随机访问效率比vector要低，内部插入和删除的效率比list要低（对应就是内部插入和删除效率比vector高，随机访问效率比list高），deque的缺点就是三者之中它所占用的内存是最大的。
### 三者的选择策略  
1. 如果你需要高效的随即存取，而不在乎插入和删除的效率，使用vector  
2. 如果你需要大量的插入和删除，而不关心随即存取，则应使用list  
3. 如果你需要随即存取，而且关心两端数据的插入和删除，则应使用deque


### map 和 unordered_map 的内部实现机制
- map内部实现了一个红黑树，该结构具有**自动排序**的功能，因此map内部的所有元素都是有序的，红黑树的每一个节点都代表着map的一个元素，因此，对于map进行的查找，删除，添加等一系列的操作都相当于是对红黑树进行这样的操作，故红黑树的效率决定了map的效率。
- unordered_map内部实现了一个哈希表，因此其元素的排列顺序是杂乱的，无序的。
- map 和 unordered_map 都是不需要键值重复而允许数值重复
### map 和 unordered_map 的优缺点
#### map 
- 优点： 
	1. 有序性，这是map结构最大的优点，其元素的有序性在很多应用中都会简化很多的操作。（按照键值从小到大排序）
	2.  红黑树，内部实现一个红黑书使得map的很多操作在$O(log(n))$的时间复杂度下就可以实现，因此效率非常的高
- 缺点： 
	1. 空间占用率高，因为map内部实现了红黑树，虽然提高了运行效率，但是因为每一个节点都需要额外保存父节点，孩子节点以及红/黑性质，使得每一个节点都占用大量的空间。
- 适用处，对于那些有顺序要求的问题，用map会更高效一些
#### unordered_map 
- 优点：因为内部实现了哈希表，因此其查找速度非常的快	
- 缺点：哈希表的建立比较耗费时间	
- 适用处，对于查找问题，unordered_map会更加高效一些，因此遇到查找问题，常会考虑一下用unordered_map。

## Dijkstra's Algorithm

​	Dijkstra's Algorithm 是使用了类似于BFS（Breadth First Search）的搜索方法解决正值[[图 (Graphs)#赋权图|赋权图]]的最短路径问题（如地铁最短路径）。对于不含负权的有向图，Dijkstra算法是目前已知的最快的单源最短路径算法。


#### 算法逻辑

1. 设定最短路径的顶点集合 $S$，该集合存放所有与起点相邻或是可达的顶点。其余顶点集合为 $U$。存放未确定其可达性的顶点。
2. 从数据集合中指定作为最短路径起点的顶点$v$，加入到 $S$ 集合中，即令$S = \{v(0)\}$。
3. 在 $U$ 集合中寻找与顶点 $v$ 相邻的顶点，并将相应的权值表示为距离，非相邻的顶点距离表示为 $\infty$。如 $U = \{a(3),b(2),c(\infty),...\}$
4. 将相邻顶点中“距离起点路径最短”的顶点放入 $S$ 集合中。根据上述的例子，此时 $S = \{v(0), b(2)\}$， $U = \{a(3),c(\infty),...\}$ 。
5. 更新 $U$集合中各顶点到 $v$ 距离：在经过上述步骤后，有可能发生以下情况：
    1. 与 $v$ 非相邻但是可达的顶点 （即能通过 $S$集合中其他顶点与 $v$ 实现连接的顶点），之前的距离被记录为 $\infty$，更新后为可达路径所经过的边的权值的总和。
    2. 之前更新过距离值的顶点，由于 $S$ 集合的顶点新增，发现了更小的可达距离，此时就需要进行相应的更新。
6. 重复上述步骤，就可以统计出顶点 $v$ 到其余点的最短距离。 

注意，每次更新距离都是以 $S$ 集合中的顶点出发，根据可达性来更新距离值的，也就是说，逻辑上我们不能从 $U$ 集合中的顶点出发去更新其他顶点的距离值，因为这些顶点的可达性并没有被确定。

[[Graphs-DirectedGraphs]] [[Graphs-WeightedGraphs]] 

## 图 (Graphs)

### 图的概念

- 图是描述多个点之间的关系的结构（多对多，且所有点都是平等的）。
- 一个图的结构可以由一个三元组 $G= (V,Ε,\gamma)$ 来描述，$V,Ε$ 都是一个有限集合， 其中：
    1. $V = \{v_1,v_2,...,v_n\}$ 是图的顶点集，其元素称作顶点或结点 *(Vertex Node or Point) *。
    2. $Ε = \{e_1, e_2, ... ,e_m \}$是图的边集，其元素称作边 *(edge, arc, line)*。
    3. 而$\gamma$ 则描述了边 $e$ 与顶点 $v$ 的关系，对于两个顶点$\{u,v\}$，和一条边$e$, 有$\gamma(e) = \{u,v\}$，称$u,v$为$e$的端点 (End points)。
- 根据 $\gamma(e) = \{u,v\}$ 所描述的具体关系，图可分为[[无向图 (Undirected Graphs)]]& [[有向图 (Directed Graphs)]]

### 赋权图 - **Weighted Graph**

上述中，无论是无向图还是有向图，他们都只描述了顶点之间的连接情况（对于有向图另外指明了方向性），但在具体问题中，我们常常需要描述两个顶点之间连接的“距离”（比如城市A->C距离为40公里，城市A->B距离为60公里），为此，我们引入权重（weight）$w(e)$来描述这种“距离”关系

对于图的边 $Ε = \{e_1, e_2, ... ,e_m\}$ 我们用$w(e)$ 或者 $w(u,v)$ 来描述边的权重，即每条边的值，即有 $W = \{w_1,w_2,...w_m\}$。 所以对于一个每条边都有对应权值的图，我们称为赋权图，用$(G,W)$ 来表示。

### 存储图的数据结构

根据实际问题，我们可以将图以邻接矩阵，邻接表，逆邻接表，十字链表的数据结构方式表示，以描述顶点与顶点之间的连接关系。比如邻接矩阵本质上是有数组方式来存储数据，用递归的方式遍历数据来描述图的结构，邻接表本质上则是用链表数组的方式存储数据。
对于每种数据结构事实上是将图的多对多关系根据其连接情况转化为树的一对多关系，而对于树形结构，我们可以通过深度优先（DFS）和广度优先（BFS）现对数据的操作。**也就是说，DFS 和 BFS 实际上是对树结构的数据进行搜索算法，所以我们所说的邻接矩阵，邻接表，逆邻接表，十字链表实际上都是将图转化为树结构来存储。**

>   需要注意的是：DFS 和 BFS 只是解决图数据搜索的一种算法，事实上，并非用DFS | BFS 算法的问题对应的数据结构都是图（也可以是树），或者说，我们在进行问题分析时，需要考虑是否有必要将数据转化成图的形式（顶点、边、以及描述顶点和边的关系），很多时候，我们需要将数据理解成简单的二维数组，但依然可以用 DFS 和 BFS 来进行数据搜索。从而解决问题， DFS ｜ BFS的学习应与图分开。这是我学习上的一个误区，特地纠正下。

[[Graphs]] [[Graphs-WeightedGraphs]] 

### 无向图 (Undirected Graphs)

无向图是$\gamma(e) = {u,v}$所描述的边与端点的关系是无向的，更具体的说，两个顶点和一条边如果满足$\gamma(e) = {u,v}$，则说明$u,v$是**相互连通**的，无向图也基于此来描述多个点中存在相互连通的关系点。（例如城市间的交通相互连接）。

对于无向图，由于它只是描述了顶点的相互连接情况，所以：

1. $\gamma(e) = \{u,v\}$和$\gamma(e) = \{u,v\}$是等价的。
2. **两个顶点之间可能存在多条边**，即$\gamma(e_1) = {u,v}$、$\gamma(e_2) = \{u,v\}$ 同时成立。这时$e_1,ε_2$称为重边或平行边 *(Parallel  edges)*。
3. 对于$\gamma(e) = \{u,v\}$，我们称$\{u,v\}$为相邻顶点(adjacent vertices，并称$e$与$\{u,v\}$ 相关联。如果$\{u=v\}$，则称$e$为一个自环。如果$e_1 \ e_2$与同一个顶点满足连接关系，则称$e_1,e_2$是相邻 *(adjacent)* 的。

在无向图中，我们用**度 $deg(v)$** 来描述与**某一顶点 $v$ 关联的边的的数**，另外自环在计度数时取2。基于此，我们称：

1. $deg(v) = 0$的顶点为孤立顶点 * (Isolated  vertex)* 
2. $deg(v) = 1$ 的点称为悬挂点  *(Pendant vertex)*，与悬挂点相连的边称为悬挂边 *(Pendant edge)* 。
3. $deg(v) = k$ 的点称为$k$度点，$deg(v) = 奇数$  的点称为奇度点， $deg(v) = 偶数$ 的点称为偶度点。

[[Graphs-UndirectedGraphs]] 

### 有向图 (Directed Graphs)

有向图是指$\gamma(e) = \{u,v\}$ 所描述的边与端点的关系是有向的，更具体的说，两个顶点和一条边如果满足 $\gamma(e) = \{u,v\}$ ，则 $u$  作为边 $e$ 的起点 *(Start point)*，$v$ 作为边 $e$ 的终点 *(End point)*。

对于有向图，由于边出了连接两个顶点，同时明确了其方向性，所以$\gamma(e) = {u,v}$和$\gamma(e) = {u,v}$不是等价的，**其余关于点与边之间的关系与无向图相同。(相邻点，相邻边)**

在有向图中，我们用**入度$(deg^-(v))$** 来描述以 $v$ 为终点的关联的有向边的数目，用**出度$(deg^+(v))$** 来描述以 $v$ 为起点的关联的有向边的数目。所以对于某个顶点，其度数 $deg(v)$：
$$
deg(v) = (deg^-(v)) + (deg^+(v))
$$
对于有向图，我们也称：

1. $deg(v) = 0$的顶点为孤立顶点* (Isolated  vertex)* 
2. $deg(v) = 1$ 的点称为悬挂点* (Pendant vertex)* ，与悬挂点相连的边称为悬挂边 *(Pendant edge) * 
3. $deg(v) = k$ 的点称为$k$度点，$deg(v) = 奇数$  的点称为奇度点， $deg(v) = 偶数$ 的点称为偶度点。

#### 有向图的多种数据结构

#### 邻接表

**邻接表（**英语**：adjacency list）**是表示了[图](https://zh.wikipedia.org/wiki/%E5%9B%BE_(%E6%95%B0%E5%AD%A6) "图 (数学)")中与每一个顶点"顶点 (图论)")相邻的边集的集合，这里的集合指的是 **无序集** 。在邻接表的表示中，对于图中的每个顶点，我们将保存所有其它与之相连的顶点（即“邻接表”）。
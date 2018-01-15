
#include "global.h"
using namespace std;

struct TreeNode
{
public:
    TreeNode (int nVertexIndexA = 0, int nVertexIndexB = 0, int nWeight = 0)
        : m_nVertexIndexA (nVertexIndexA),
          m_nVertexIndexB (nVertexIndexB),
          m_nWeight (nWeight)
    { }
public:
    int m_nVertexIndexA ;
    int m_nVertexIndexB ;
    int m_nWeight ;
} ;

class MST_Prim
{
public:
    MST_Prim (const vector<vector<int> >& vnGraph)
    {
        m_nvGraph = vnGraph ;
        m_nNodeCount = (int)m_nvGraph.size () ;
    }
    void DoPrim ()
    {
        // 是否被访问标志
        vector<bool> bFlag (m_nNodeCount, false) ;
        bFlag[0] = true ;

        int nMaxIndexA ;
        int nMaxIndexB ;
        int j = 0 ;
        while (j < m_nNodeCount - 1) {
            int nMaxWeight = numeric_limits<int>::max () ;
            // 找到当前最短路径
            int i = 0 ;
            while (i < m_nNodeCount) {
                if (!bFlag[i]) {
                    ++ i ;
                    continue ;
                }
                for (int j = 0; j < m_nNodeCount; ++ j) {
                    if (!bFlag[j] && nMaxWeight > m_nvGraph[i][j]) {
                        nMaxWeight = m_nvGraph[i][j] ;
                        nMaxIndexA = i ;
                        nMaxIndexB = j ;
                    }
                }
                ++ i ;
            }
            bFlag[nMaxIndexB] = true ;
            m_tnMSTree.push_back (TreeNode(nMaxIndexA, nMaxIndexB, nMaxWeight)) ;
            ++ j ;
        }
        // 输出结果
        for (vector<TreeNode>::const_iterator ite = m_tnMSTree.begin() ;
             ite != m_tnMSTree.end() ;
             ++ ite ) {
            cout << (*ite).m_nVertexIndexA << "->"
                 << (*ite).m_nVertexIndexB << " : "
                 << (*ite).m_nWeight << endl ;
        }
    }
private:
    vector<vector<int> > m_nvGraph ;    // 无向连通图
    vector<TreeNode>    m_tnMSTree ;    // 最小生成树
    int    m_nNodeCount ;
} ;

int testSpanningTree11()
{
    const int cnNodeCount = 6 ;
    vector<vector<int> > graph (cnNodeCount) ;
    for (size_t i = 0; i < graph.size() ; ++ i) {
        graph[i].resize (cnNodeCount, numeric_limits<int>::max()) ;
    }
    graph[0][1]= 6 ;
    graph[0][2] = 1 ;
    graph[0][3] = 5 ;
    graph[1][2] = 5 ;
    graph[1][4] = 3 ;
    graph[2][3] = 5 ;
    graph[2][4] = 6 ;
    graph[2][5] = 4 ;
    graph[3][5] = 2 ;
    graph[4][5] = 6 ;

    graph[1][0]= 6 ;
    graph[2][0] = 1 ;
    graph[3][0] = 5 ;
    graph[2][1] = 5 ;
    graph[4][1] = 3 ;
    graph[3][2] = 5 ;
    graph[4][2] = 6 ;
    graph[5][2] = 4 ;
    graph[5][3] = 2 ;
    graph[5][4] = 6 ;

    MST_Prim mstp (graph) ;
    mstp.DoPrim () ;

    return 0 ;
}
#include <bits/stdc++.h>
using namespace std;
ifstream fin("date.in");
ofstream fout("date.out");
#define nmax 103
int n,m,a,b;
int eticheta[nmax];
char sir[nmax],caracter;
int stare_initiala,nr_stari,x,nr_tranzitii,cnt;
bool stari_finale[nmax],eliminare[nmax],viz[nmax];
set < int >solutie,adiacenta[nmax][nmax],matrice_NFA[nmax][nmax],finale_NFA,inchidere[nmax];
vector < pair <int, int > > stari_eliminate;
void citire()
{
    fin>>n>>m;
    fin.get();
    for(int i=0;i<m;++i)
        fin>>sir[i];
    fin>>stare_initiala>>nr_stari;
    for(int i=1; i<=nr_stari; ++i)
    {
        fin>>x;
        stari_finale[x]=true;
    }
    fin>>nr_tranzitii;
    for(int i=1; i<=nr_tranzitii; ++i)
    {
        fin>>a>>caracter>>b;
        adiacenta[a][int(caracter)].insert(b);
    }
}
void inchidere__stari_finale()
{
    vector < int > stari;
    for(int i = 0; i < n; i++)
    {
        inchidere[i].insert(i);        
        stari.clear();
        memset(viz,0,sizeof(viz));
        viz[i] = true;
        for(set < int > ::iterator it = adiacenta[i][int('$')].begin(); it !=adiacenta[i][int('$')].end(); it++)
        {
            stari.push_back(*it);
            viz[*it] = true;
        }
        for(int q = 0; q < stari.size(); q++)
        {
            inchidere[i].insert(stari[q]);     
            for(set < int > ::iterator it = adiacenta[stari[q]][int('$')].begin(); it != adiacenta[stari[q]][int('$')].end(); it++)
                if(!viz[*it])
                {
                    viz[*it] = true;
                    stari.push_back(*it);       
                }
        }

        for(set < int > ::iterator it = inchidere[i].begin(); it != inchidere[i].end(); it++)
            if(!stari_finale[*it])
            {
                finale_NFA.insert(i);
                break;
            }
    }
}
void NFA()
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
        {
            solutie.clear();
            for(set < int > ::iterator it = inchidere[i].begin(); it != inchidere[i].end(); it++)
                for(set < int > ::iterator b = adiacenta[*it][int(sir[j])].begin(); b!= adiacenta[*it][int(sir[j])].end(); b++)
                    solutie.insert(*b);    
            for(set < int > ::iterator it = solutie.begin(); it != solutie.end(); it++)
                matrice_NFA[i][int(sir[j])].insert(inchidere[*it].begin(), inchidere[*it].end());
        }
}

void eliminare_stari_redundante()
{
    for(int i=0;i<=n;++i)
        eticheta[i]=-1;
    for(int i=0; i<n; ++i)
        if(!eliminare[i])
        {
            eticheta[i]=cnt++;
            for(int j=i+1; j<n; ++j)
                if(!eliminare[j])
                {
                    int q;
                    for(q=0; q<m; ++q)
                        if(matrice_NFA[i][int(sir[q])] !=matrice_NFA[j][int(sir[q])])
                            break;
                    if(q == m)
                    {
                        stari_eliminate.push_back({j,i});    
                        eliminare[j] = true;     
                    }
                }
        }

    for(int i = 0; i < n; i++)
        if(!eliminare[i])
            for(int j = 0; j < m; j++)
                for( vector <pair <int, int> >::iterator  b = stari_eliminate.begin(); b != stari_eliminate.end(); b++)
                {
                    set<int>::iterator it;
                    it = matrice_NFA[i][int(sir[j])].find(b->first);    
                    if(it != matrice_NFA[i][int(sir[j])].end())
                    {
                        matrice_NFA[i][int(sir[j])].erase(it);
                        matrice_NFA[i][int(sir[j])].insert(b->second);
                    }
                }
    for(vector <pair <int, int> >::iterator i = stari_eliminate.begin(); i != stari_eliminate.end(); i++)
    {
        set<int>::iterator it;
        it = finale_NFA.find(i->first);
        if(it != finale_NFA.end())
        {
            finale_NFA.erase(it);
            finale_NFA.insert(i->second);
        }
    }
}
int main()
{
    citire();
    inchidere__stari_finale();
    NFA();
    eliminare_stari_redundante();
    return 0;
}

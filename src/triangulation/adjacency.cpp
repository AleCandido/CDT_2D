// ##### ADJACENCY LIST #####

void Triangulation::produce_adjacency_list(string filename)
{
    ofstream of(filename, ios::binary | ios::out);
    
    produce_adjacency_list(of);
    of.close();
}


void Triangulation::produce_adjacency_list(std::ofstream& output)
{
    int len=list2.size();
    output.write((char*)&len, sizeof(len));
    
    for(auto x: list2){
        for(int i=0; i<3; i++){
            int pos = x.dync_triangle()->t[i]->id;
            output.write((char*)&pos, sizeof(pos));
        }
    }
}

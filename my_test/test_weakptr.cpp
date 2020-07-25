#include <memory>
#include <vector>


using namespace std;


int main(int argc, char* argv[])
{
    shared_ptr<vector<int> > array(new vector<int>(5));
    weak_ptr<vector<int> > weak_array(array);
    printf("user count = %ld \n", array.use_count());
    array.reset();
    if (weak_array.expired())
    {
        printf("array expired!\n");
    }
    else 
        printf("array done !\n");
    
    return 1;
}
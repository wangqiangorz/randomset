#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<random>

class RandomSet {
public:

    RandomSet(bool is_repeatable, int random_seed) {
        is_repeatable = is_repeatable;
        size = 0;
        e.seed(random_seed);
    }

    bool find(int val) {
        return !item_idxs_with_val[val].empty();
    }

    bool insert(int val) {

        // 如果不可重复直接返回
        if(!is_repeatable && !item_idxs_with_val[val].empty()){
            return false;
        }

        // 更新存储列表和哈希表
        item_val_list.push_back(val);
        item_idxs_with_val[val].insert(size);
        ++ size;
        return true;
    }

    bool remove(int val) {

        // 不存在这个元素 非法操作
        if(item_idxs_with_val[val].empty()){
            return false;
        }
        // 随便找一个这个元素的一个位置
        int val_idx = *item_idxs_with_val[val].begin();

        // 删除这个位置
        item_idxs_with_val[val].erase(val_idx);

        // 获取item最后一个元素，并且与上述元素交换,同时更新索引哈希表
        int last_val = item_val_list.back();
        if(val_idx != size - 1){
            item_val_list[val_idx] = last_val;
            item_val_list.pop_back();
            item_idxs_with_val[last_val].erase(size - 1);
            item_idxs_with_val[last_val].insert(val_idx);
        }else{
            item_val_list.pop_back();
            item_idxs_with_val[last_val].erase(size - 1);
        }

        -- size;
        return true;
    }

    int getRandom() {
        // 当个数为0是，返回-1 表示不存在
        if(size == 0){
            return -1;
        }
        return item_val_list[e() % size];
    }
private:

    bool is_repeatable;
    std::unordered_map<int, std::unordered_set<int>> item_idxs_with_val;
    std::vector<int> item_val_list;
    int size;
    std::default_random_engine e;

};

/**
 * Your RandomSet object will be instantiated and called as such:
 * RandomSet* obj = new RandomSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */
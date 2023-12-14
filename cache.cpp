#include <iostream>
#include <vector>

const int MEM_SIZE = 524288;
const int CACHE_LINE_SIZE = 32;
const int CACHE_WAY = 4;
const int CACHE_SETS_COUNT = 16;
const int CACHE_IDX_LEN = 4;
const int CACHE_OFFSET_LEN = 5;
const int START_POS = 262144;
const int MX = 2147483646;

using namespace std;

struct cache_line {
    int tag;
    vector<int> data;

    cache_line() {
        tag = 0;
        data = vector<int>(CACHE_LINE_SIZE);
    }
};

struct memory {
    vector<int> mem;
    vector<bool> used;

    memory() {
        mem.resize(MEM_SIZE);
        used.resize(MEM_SIZE);
    }

    int initial(int size, vector<int> &values, int value_type) {
        int address = 0;
        for (int i = START_POS; i < MEM_SIZE; i++) {
            if (!used[i]) {
                for (int j = 0; j < size; j++) {
                    int m = value_type / 8;
                    for (int z = 0; z < m; z++) {
                        mem[i + j * m + z] = values[j] % (1 << 8);
                        values[j] /= (1 << 8);
                        used[i + j * m + z] = true;
                    }
                }
                address = i;
                break;
            }
        }
        return address;
    }

    cache_line *read(int address) {
        auto *ans = new cache_line();
        address = (address >> CACHE_OFFSET_LEN);
        ans->tag = address;
        address = (address << CACHE_OFFSET_LEN);
        for (int i = 0; i < CACHE_LINE_SIZE; i++) {
            ans->data[i] = mem[address + i];
        }
        return ans;
    }

    void write(cache_line *line) {
        int address = line->tag;
        address = address << CACHE_OFFSET_LEN;
        for (int i = 0; i < CACHE_LINE_SIZE; i++) {
            mem[address + i] = line->data[i];
        }
    }
};

struct block {
    vector<cache_line *> cache_lines;
    vector<bool> modified;
    vector<int> times;
    int timer;

    block() {
        modified.resize(CACHE_WAY);
        times.resize(CACHE_WAY);
        timer = 1;
        for (int i = 0; i < CACHE_WAY; i++) {
            cache_lines.push_back(new cache_line());
        }
    }
};

struct cache {
    vector<block *> blocks;

    cache() {
        for (int i = 0; i < CACHE_SETS_COUNT; i++) {
            blocks.push_back(new block());
        }
    }

    pair<bool, int> read_LRU(int address, int value_type) {
        int index = (address >> CACHE_OFFSET_LEN) % (1 << CACHE_IDX_LEN);
        for (int i = 0; i < blocks[index]->cache_lines.size(); i++) {
            if (blocks[index]->cache_lines[i]->tag == (address >> CACHE_OFFSET_LEN)) {
                int ans = 0;
                blocks[index]->times[i] = blocks[index]->timer++;
                for (int j = value_type / 8 - 1; j >= 0; j--) {
                    ans *= (1 << 8);
                    ans += blocks[index]->cache_lines[i]->data[address % (1 << CACHE_OFFSET_LEN) + j];
                }
                return {true, ans};
            }
        }
        return {false, 0};
    }

    bool write_LRU(int address, int data, int value_type) {
        int index = (address >> CACHE_OFFSET_LEN) % (1 << CACHE_IDX_LEN);
        for (int i = 0; i < blocks[index]->cache_lines.size(); i++) {
            if (blocks[index]->cache_lines[i]->tag == (address >> CACHE_OFFSET_LEN)) {
                blocks[index]->modified[i] = true;
                blocks[index]->times[i] = blocks[index]->timer++;
                for (int j = 0; j < value_type / 8; j++) {
                    blocks[index]->cache_lines[i]->data[address % (1 << CACHE_OFFSET_LEN) + j] = data % (1 << 8);
                    data /= (1 << 8);
                }
                return true;
            }
        }
        return false;
    }

    pair<bool, cache_line *> add_data_LRU(int address, cache_line *new_line) {
        int index = (address >> CACHE_OFFSET_LEN) % (1 << CACHE_IDX_LEN);
        int mn = MX;
        int ind = 0;
        for (int i = 0; i < blocks[index]->times.size(); i++) {
            if (blocks[index]->times[i] < mn) {
                mn = blocks[index]->times[i];
                ind = i;
            }
        }
        pair<bool, cache_line *> ans = {false, nullptr};
        if (blocks[index]->modified[ind]) {
            ans = {true, blocks[index]->cache_lines[ind]};
        }
        blocks[index]->cache_lines[ind] = new_line;
        blocks[index]->modified[ind] = false;
        blocks[index]->times[ind] = blocks[index]->timer++;
        return ans;
    }

    void check(int index, int i) {
        int cnt = 0;
        for (auto el : blocks[index]->times) {
            cnt += el;
        }
        if (cnt == blocks[index]->times.size()) {
            for (int j = 0; j < blocks[index]->times.size(); j++) {
                if (j != i) {
                    blocks[index]->times[j] = 0;
                }
            }
        }
    }

    pair<bool, int> read_pLRU(int address, int value_type) {
        int index = (address >> CACHE_OFFSET_LEN) % (1 << CACHE_IDX_LEN);
        for (int i = 0; i < blocks[index]->cache_lines.size(); i++) {
            if (blocks[index]->cache_lines[i]->tag == (address >> CACHE_OFFSET_LEN)) {
                int ans = 0;
                blocks[index]->times[i] = 1;
                check(index, i);
                for (int j = value_type / 8 - 1; j >= 0; j--) {
                    ans *= (1 << 8);
                    ans += blocks[index]->cache_lines[i]->data[address % (1 << CACHE_OFFSET_LEN) + j];
                }
                return {true, ans};
            }
        }
        return {false, 0};
    }

    bool write_pLRU(int address, int data, int value_type) {
        int index = (address >> CACHE_OFFSET_LEN) % (1 << CACHE_IDX_LEN);
        for (int i = 0; i < blocks[index]->cache_lines.size(); i++) {
            if (blocks[index]->cache_lines[i]->tag == (address >> CACHE_OFFSET_LEN)) {
                blocks[index]->modified[i] = true;
                blocks[index]->times[i] = 1;
                check(index, i);
                for (int j = 0; j < value_type / 8; j++) {
                    blocks[index]->cache_lines[i]->data[address % (1 << CACHE_OFFSET_LEN) + j] = data % (1 << 8);
                    data /= (1 << 8);
                }
                return true;
            }
        }
        return false;
    }

    pair<bool, cache_line *> add_data_pLRU(int address, cache_line *new_line) {
        int index = (address >> CACHE_OFFSET_LEN) % (1 << CACHE_IDX_LEN);
        int ind = 0;
        for (int i = 0; i < blocks[index]->times.size(); i++) {
            if (!blocks[index]->times[i]) {
                ind = i;
                break;
            }
        }
        pair<bool, cache_line *> ans = {false, nullptr};
        if (blocks[index]->modified[ind]) {
            ans = {true, blocks[index]->cache_lines[ind]};
        }
        blocks[index]->cache_lines[ind] = new_line;
        blocks[index]->modified[ind] = false;
        blocks[index]->times[ind] = 1;
        check(index, ind);
        return ans;
    }
};

struct solve {
    memory *m;
    cache *ch;
    int type;

    solve(int t) {
        type = t;
        m = new memory();
        ch = new cache();
    }

    int initial(int size, vector<int> values, int value_type) {
        return m->initial(size, values, value_type);
    }

    int sum(int element1, int element2, int &tact) {
        tact++;
        return element1 + element2;
    }

    int mult(int element1, int element2, int &tact) {
        tact += 5;
        return element1 * element2;
    }

    void update(int address, int &tact) {
        tact += 124;
        auto line = m->read(address);
        pair<bool, cache_line *> data;
        if (type == 1) {
            data = ch->add_data_LRU(address, line);
        } else {
            data = ch->add_data_pLRU(address, line);
        }
        if (data.first) {
            tact += 103;
            m->write(data.second);
        }
    }

    int get(int address, int value_type, int &tact, int &cache_hits, int &operations) {
        pair<bool, int> p;
        if (type == 1) {
            p = ch->read_LRU(address, value_type);
        } else {
            p = ch->read_pLRU(address, value_type);
        }
        operations++;
        if (p.first) {
            tact += 8;
            cache_hits++;
            return p.second;
        }
        update(address, tact);
        if (type == 1) {
            p = ch->read_LRU(address, value_type);
        } else {
            p = ch->read_pLRU(address, value_type);
        }
        return p.second;
    }

    void set(int address, int value, int value_type, int &tact, int &cache_hits, int &operations) {
        operations++;
        bool flag;
        if (type == 1) {
            flag = ch->write_LRU(address, value, value_type);
        } else {
            flag = ch->write_pLRU(address, value, value_type);
        }
        if (flag) {
            cache_hits++;
            tact += 8;
        } else {
            update(address, tact);
        }
    }
};

signed main() {
    int n = 60, m = 64, k = 32;
    auto *st = new solve(1);
    auto *pst = new solve(2);
    int tact = 0;
    int cache_hits = 0;
    int operations = 0;
    int p_tact = 0;
    int p_cache_hits = 0;
    int p_operations = 0;
    vector<vector<int>> a(m, vector<int>(k));
    vector<vector<int>> b(k, vector<int>(n));
    vector<vector<int>> c(m, vector<int>(n));

    vector<int> start_a(m);
    vector<int> start_b(k);
    vector<int> start_c(m);

    for (int i = 0; i < m; i++) {
        start_a[i] = st->initial(k, a[i], 8);
        pst->initial(k, a[i], 8);
    }
    for (int i = 0; i < k; i++) {
        start_b[i] = st->initial(n, b[i], 16);
        pst->initial(n, b[i], 16);
    }
    for (int i = 0; i < m; i++) {
        start_c[i] = st->initial(n, c[i], 32);
        pst->initial(n, c[i], 32);
    }

    int temp = 0;
    int pa = start_a[temp];
    int pc = start_c[temp];
    tact += 3;
    p_tact += 3;
    for (int y = 0; y < m; y++) {
        tact++;
        p_tact++;
        for (int x = 0; x < n; x++) {
            int temp2 = 0;
            int pb = start_b[temp2];
            tact += 2;
            p_tact += 2;
            int res = 0;
            int res2 = 0;
            for (int z = 0; z < k; z++) {
                int val1 = st->get(pa + z, 8, tact, cache_hits, operations);
                int val2 = st->get(pb + x * 2, 16, tact, cache_hits, operations);
                int p_val1 = pst->get(pa + z, 8, p_tact, p_cache_hits, p_operations);
                int p_val2 = pst->get(pb + x * 2, 16, p_tact, p_cache_hits, p_operations);
                res = st->sum(res, st->mult(val1, val2, tact), tact);
                res2 = pst->sum(res2, pst->mult(p_val1, p_val2, p_tact), p_tact);
                temp2++;
                tact += 3;
                p_tact += 3;
                pb = start_b[temp2];
            }
            st->set(pc + 4 * x, res, 32, tact, cache_hits, operations);
            pst->set(pc + 4 * x, res2, 32, p_tact, p_cache_hits, p_operations);
            tact += 2;
            p_tact += 2;
        }
        temp++;
        pa = start_a[temp];
        pc = start_c[temp];
        tact += 4;
        p_tact += 4;
    }
    printf("LRU:\thit perc. %3.4f%%\ttime: %d\n", float((float)100 * (float)cache_hits/(float)operations), tact);
    printf("pLRU:\thit perc. %3.4f%%\ttime: %d\n", float((float)100 * (float)p_cache_hits/(float)p_operations), p_tact);
}

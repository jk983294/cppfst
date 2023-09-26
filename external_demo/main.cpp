#include <fststore.h>
#include <fsttable.h>

static void WriteTable(FstTable& table, const std::string& file_name, const int compression) {
    FstStore fstStore(file_name);
    fstStore.fstWrite(table, compression);
    printf("write %s success", file_name.c_str());
}

int main() {
    std::string path_ = "/tmp/data2.fst";
    int nrOfRows = 10;

    std::vector<double> d_vec(nrOfRows);
    std::vector<int> i_vec(nrOfRows);
    std::vector<std::string> s_vec(nrOfRows);
    for (int i = 0; i < nrOfRows; ++i) {
        d_vec[i] = i;
        i_vec[i] = i;
        s_vec[i] = std::to_string(i);
    }

    FstTable fstTable(nrOfRows);
    fstTable.InitTable(3, nrOfRows);

    IntVectorAdapter intVec(i_vec);
    fstTable.SetIntegerColumn(&intVec, 0);

    StringColumn strColumn(s_vec);
    fstTable.SetStringColumn(static_cast<IStringColumn*>(&strColumn), 1);

    DoubleVectorAdapter doubleVec(d_vec);
    fstTable.SetDoubleColumn(&doubleVec, 2);

    // Set column names
    std::vector<std::string> colnames = {"integer_n", "character_n", "double_n"};
    StringArray colNames(colnames);
    fstTable.SetColumnNames(colNames);

    WriteTable(fstTable, path_, 30);
    return 0;
}

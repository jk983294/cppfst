#include <fststore.h>
#include <fsttable.h>

static void WriteTable(FstTable& table, const std::string& file_name, const int compression) {
    FstStore fstStore(file_name);
    fstStore.fstWrite(table, compression);
    printf("write %s success", file_name.c_str());
}

int main() {
    std::string path_ = "/tmp/data1.fst";
    int nrOfRows = 10;
    FstTable fstTable(nrOfRows);
    fstTable.InitTable(3, nrOfRows);

    // Add integer column
    IntVectorAdapter intVec(nrOfRows, FstColumnAttribute::INT_32_BASE, 0);
    for (int i = 0; i < nrOfRows; ++i) {
        (intVec.Data())[i] = i;
    }
    fstTable.SetIntegerColumn(&intVec, 0);

    // Add string column
    StringColumn strColumn{};
    strColumn.AllocateVec(nrOfRows);
    strColumn.SetEncoding(StringEncoding::UTF8);
    std::shared_ptr<StringVector> strColVec = strColumn.StrVector();
    std::vector<std::string>* strVec = strColVec->StrVec();

    for (int i = 0; i < nrOfRows; ++i) {
        (*strVec)[i] = std::to_string(i);
    }

    fstTable.SetStringColumn(static_cast<IStringColumn*>(&strColumn), 1);

    // Add double column
    DoubleVectorAdapter doubleVec(nrOfRows, FstColumnAttribute::DOUBLE_64_BASE, 0);
    for (int i = 0; i < nrOfRows; ++i) {
        (doubleVec.Data())[i] = 10. + 0.1 * i;
    }
    fstTable.SetDoubleColumn(&doubleVec, 2);

    // Set column names
    std::vector<std::string> colnames = {"Integer", "Character", "Double"};
    StringArray colNames(colnames);
    fstTable.SetColumnNames(colNames);

    WriteTable(fstTable, path_, 30);
    return 0;
}

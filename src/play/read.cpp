#include <columnfactory.h>
#include <fststore.h>
#include <fsttable.h>

int main() {
    std::string path_ = "/tmp/data2.fst";
    FstTable table;
    ColumnFactory columnFactory;
    std::vector<int> keyIndex;
    StringArray selectedCols;

    std::unique_ptr<StringColumn> col_names(new StringColumn());
    FstStore fstStore(path_);
    fstStore.fstRead(table, nullptr, 1, -1, &columnFactory, keyIndex, &selectedCols, &*col_names);

    uint64_t rows = table.NrOfRows();
    printf("rows=%zu\n", rows);
    for (uint64_t i = 0; i < selectedCols.Length(); ++i) {
        std::shared_ptr<DestructableObject> column;
        FstColumnType type;
        std::string colName;
        short int colScale;
        std::string annotation;
        table.GetColumn(i, column, type, colName, colScale, annotation);
        printf("type=%d, name=%s, scale=%d, Annotation=%s\ndata=", type, selectedCols.GetElement(i), colScale,
               annotation.c_str());

        if (type == FstColumnType::INT_32) {
            auto i_vec = std::dynamic_pointer_cast<IntVector>(column);
            auto ptr = i_vec->Data();
            for (uint64_t j = 0; j < rows; ++j) {
                printf("%d ", ptr[j]);
            }
            printf("\n");
        } else if (type == FstColumnType::DOUBLE_64) {
            auto i_vec = std::dynamic_pointer_cast<DoubleVector>(column);
            auto ptr = i_vec->Data();
            for (uint64_t j = 0; j < rows; ++j) {
                printf("%f ", ptr[j]);
            }
            printf("\n");
        } else if (type == FstColumnType::CHARACTER) {
            auto i_vec = std::dynamic_pointer_cast<StringVector>(column);
            auto ptr = i_vec->StrVec();
            for (uint64_t j = 0; j < rows; ++j) {
                printf("%s ", (*ptr)[j].c_str());
            }
            printf("\n");
        }
    }
    return 0;
}

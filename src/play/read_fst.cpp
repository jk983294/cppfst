#include <columnfactory.h>
#include <fststore.h>
#include <fsttable.h>
#include <getopt.h>

static void help() {
    printf("read_fst -f file -t 1\n");
    printf("\t\t-f\t\t\tfile\n");
    printf("\t\t-t\t\t\t1: read schema 2: read data\n");
}

static std::string FstColumnType2String(FstColumnType type) {
    switch (type) {
        case FstColumnType::CHARACTER: return "CHARACTER";
        case FstColumnType::FACTOR: return "FACTOR";
        case FstColumnType::INT_32: return "INT_32";
        case FstColumnType::DOUBLE_64: return "DOUBLE_64";
        case FstColumnType::BOOL_2: return "BOOL_2";
        case FstColumnType::INT_64: return "INT_64";
        case FstColumnType::BYTE: return "BYTE";
        case FstColumnType::BYTE_BLOCK: return "BYTE_BLOCK";
        default: return "UNKNOWN";
    }
}

int main(int argc, char** argv) {
    int f_code = 1; // 1: read schema 2: read data
    std::string path_ = "/tmp/data2.fst";
    int opt;
    while ((opt = getopt(argc, argv, "t:f:h")) != -1) {
        switch (opt) {
            case 't':
                f_code = std::stoi(optarg);
                break;
            case 'f':
                path_ = std::string(optarg);
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

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
        printf("type=%s, name=%s, scale=%d, Annotation=%s\n", FstColumnType2String(type).c_str(),
               selectedCols.GetElement(i), colScale, annotation.c_str());

        if (f_code == 1) continue;
        printf("data=");
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

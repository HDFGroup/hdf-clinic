
#include "hdf5.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DFLT H5P_DEFAULT
#define DDFLT DFLT, DFLT
#define DDDFLT DDFLT, DFLT

typedef struct {
    int a;
    float b;
    double c;
} dset2_t;

typedef struct {
    int a[4];
    float b[5][6];
} type1_t;

int H5dtor(unsigned num_args, ...)
{
    int retval = 0;
    va_list arguments;
    va_start(arguments, num_args);
    for (unsigned i = 0; i < num_args; i++)
        retval += H5Idec_ref(va_arg(arguments, hid_t));
    va_end(arguments);
    return retval;
}

int main(void)
{
    hid_t file = H5Fcreate("example.h5", H5F_ACC_TRUNC, DDFLT);

    { // Create the root group's string attribute
        hid_t space = H5Screate(H5S_SCALAR);
        hid_t type = H5Tcopy(H5T_C_S1);
        H5Tset_size(type, 17);
        H5Tset_strpad(type, H5T_STR_NULLTERM);
        hid_t attr = H5Acreate(file, "attr1", type, space, DDFLT);
        H5Awrite(attr, type, "string attribute");
        H5dtor(3, attr, type, space);
    }
    
    { // Create /dset1
        hid_t space = H5Screate_simple(2, (hsize_t[]){10,10}, NULL);
        hid_t dset = H5Dcreate(file, "dset1", H5T_STD_I32BE, space, DDDFLT);

        int data[10][10];
        for(int i = 0; i < 10; ++i)
            for(int j = 0; j < 10; ++j)
                data[i][j] = j;
        H5Dwrite(dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, DFLT, data);
        H5dtor(2, dset, space);
    }

    { // Create the compound dataset /dset2
        hid_t dset2_filetype = H5Tcreate(H5T_COMPOUND, sizeof(dset2_t));
        H5Tinsert(dset2_filetype, "a", HOFFSET(dset2_t, a), H5T_STD_I32BE);
        H5Tinsert(dset2_filetype, "b", HOFFSET(dset2_t, b), H5T_IEEE_F32BE);
        H5Tinsert(dset2_filetype, "c", HOFFSET(dset2_t, c), H5T_IEEE_F64BE);
        hid_t space = H5Screate_simple(1, (hsize_t[]){5}, NULL);
        hid_t dset = H5Dcreate(file, "dset2", dset2_filetype, space, DDDFLT);

        hid_t dset2_memtype = H5Tget_native_type(dset2_filetype, H5T_DIR_DEFAULT);
        dset2_t dset2_data[5] = {
            { 1, 0.1f, 0.01 },
            { 2, 0.2f, 0.02 },
            { 3, 0.3f, 0.03 },
            { 4, 0.4f, 0.04 },
            { 5, 0.5f, 0.05 }
        };
        H5Dwrite(dset, dset2_memtype, H5S_ALL, H5S_ALL, DFLT, dset2_data);
        H5dtor(4, dset2_memtype, dset, space, dset2_filetype);
    }

    { // Create the datatype object /type1
        hid_t array_type_a = H5Tarray_create(H5T_STD_I32BE, 1, (hsize_t[]){4});
        hid_t array_type_b = H5Tarray_create(H5T_IEEE_F32BE, 2, (hsize_t[]){5, 6});
        size_t size_a = H5Tget_size(array_type_a);
        size_t type1_size = size_a + H5Tget_size(array_type_b);
        hid_t type1 = H5Tcreate(H5T_COMPOUND, type1_size);
        H5Tinsert(type1, "a", 0, array_type_a);
        H5Tinsert(type1, "b", size_a, array_type_b);
        H5Tcommit(file, "type1", type1, DDDFLT);
        H5dtor(3, array_type_a, array_type_b, type1);
    }

    { // Create a new group and /group1/dset3 (aka. /group2/dset3)
        hid_t group1 = H5Gcreate(file, "group1", DDDFLT);
        H5Oset_comment_by_name(file, "group1", "This is a comment for group1", DFLT);

        hid_t dset3_file_type = H5Topen(file, "type1", DFLT);
        hid_t space = H5Screate_simple(1, (hsize_t[]){5}, NULL);
        hid_t dset = H5Dcreate(group1, "dset3", dset3_file_type, space, DDDFLT);

        hid_t dset3_mem_type = H5Tget_native_type(dset3_file_type, H5T_DIR_DEFAULT);
        type1_t data[5];
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 4; ++j) {
                data[i].a[j] = j;
            }
            for (int r = 0; r < 5; ++r) {
                for (int c = 0; c < 6; ++c) {
                    data[i].b[r][c] = 0.1f * (r + 1);
                }
            }
        }
        H5Dwrite(dset, dset3_mem_type, H5S_ALL, H5S_ALL, DFLT, data);
        H5dtor(5, dset3_mem_type, dset, space, dset3_file_type, group1);
    }

    { // Create the VLEN dataset /dset3
        hid_t vlen_type = H5Tvlen_create(H5T_STD_I32LE);
        hid_t space = H5Screate_simple(1, (hsize_t[]){4}, NULL);
        hid_t dset = H5Dcreate(file, "dset3", vlen_type, space, DDDFLT);

        int data[] = {0, 10, 11, 20, 21, 22, 30, 31, 32, 33};
        hvl_t vdata[] = {
            { .len = 1, .p = data },
            { .len = 2, .p = data+1 },
            { .len = 3, .p = data+3 },
            { .len = 4, .p = data+6 },
        };
        H5Dwrite(dset, vlen_type, H5S_ALL, H5S_ALL, DFLT, vdata);
        H5dtor(3, dset, space, vlen_type);
    }
    
    H5Lcreate_hard(file, "group1", file, "group2", DDFLT);
    H5Lcreate_soft("somevalue", file, "slink1", DDFLT);

    return H5Fclose(file);
}
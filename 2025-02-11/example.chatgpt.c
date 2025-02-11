/* example.c
 *
 * This program creates an HDF5 file ("example.h5") with the following objects:
 *
 *   - A root attribute "attr1" holding the string "string attribute"
 *   - Dataset "dset1": a 10×10 array of 32‐bit big–endian integers with each row [0,…,9]
 *   - Dataset "dset2": a 1D compound dataset (length 5) with three members:
 *         "a" (32–bit big–endian int),
 *         "b" (32–bit big–endian float), and
 *         "c" (64–bit big–endian double)
 *   - A named datatype “type1” (a compound type with two array members)
 *   - Group "group1" (with a comment) that contains dataset "dset3" whose datatype is the committed
 *     type “/type1”. Each element is a compound with field "a" (an array of 4 ints) and "b"
 *     (a 5×6 array of floats).
 *   - Dataset "dset3" (in the root group) is a variable–length dataset of 32–bit little–endian integers.
 *   - Group "group2" is a hard link to "group1".
 *   - A soft link "slink1" pointing to "somevalue".
 *
 * The h5dump output of the produced file should look like the one in the question.
 */

#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "example.h5"

typedef struct {
    int a;
    float b;
    double c;
} dset2_t;

typedef struct {
    int a[4];
    float b[5][6];
} type1_t;

int main(void)
{
    /* HDF5 handles */
    hid_t file_id, space_id, dset_id, attr_id;
    hid_t str_type;
    hid_t dset2_filetype, dset2_memtype;
    hid_t type1, array_type_a, array_type_b;
    hid_t group1_id, dset3_type_id;
    hid_t vlen_type;
    herr_t status;

    /* Loop counters */
    int i, j, r, c;

    /*-------------------------------------------------------------------------
     * Create file.
     *-----------------------------------------------------------------------*/
    file_id = H5Fcreate(FILENAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    /*-------------------------------------------------------------------------
     * Create an attribute "attr1" on the root group (i.e. file_id)
     * with a fixed–length string of size 17.
     *-----------------------------------------------------------------------*/
    space_id = H5Screate(H5S_SCALAR);
    str_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(str_type, 17);
    H5Tset_strpad(str_type, H5T_STR_NULLTERM);

    attr_id = H5Acreate2(file_id, "attr1", str_type, space_id,
                          H5P_DEFAULT, H5P_DEFAULT);
    {
        const char *attr_data = "string attribute";
        status = H5Awrite(attr_id, str_type, attr_data);
    }
    H5Aclose(attr_id);
    H5Sclose(space_id);
    H5Tclose(str_type);

    /*-------------------------------------------------------------------------
     * Create dataset "dset1": 10x10 of H5T_STD_I32BE.
     * Fill each row with 0,1,...,9.
     *-----------------------------------------------------------------------*/
    {
        hsize_t dims[2] = {10, 10};
        int data[10][10];

        /* Fill each row with 0,1,...,9 */
        for(i = 0; i < 10; i++)
            for(j = 0; j < 10; j++)
                data[i][j] = j;

        space_id = H5Screate_simple(2, dims, NULL);
        dset_id = H5Dcreate2(file_id, "dset1", H5T_STD_I32BE, space_id,
                             H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        /* Write using native ints (HDF5 will perform the conversion) */
        status = H5Dwrite(dset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, data);

        H5Dclose(dset_id);
        H5Sclose(space_id);
    }

    /*-------------------------------------------------------------------------
     * Create dataset "dset2": a 1D compound dataset (5 elements).
     * Compound type: members "a" (H5T_STD_I32BE), "b" (H5T_IEEE_F32BE),
     * and "c" (H5T_IEEE_F64BE).
     *
     * We define two compound types: one for file (with standard types) and one
     * for memory (using native types) to let HDF5 perform conversion.
     *-----------------------------------------------------------------------*/
    {
        hsize_t dim = 5;
        dset2_t dset2_data[5] = {
            { 1, 0.1f, 0.01 },
            { 2, 0.2f, 0.02 },
            { 3, 0.3f, 0.03 },
            { 4, 0.4f, 0.04 },
            { 5, 0.5f, 0.05 }
        };

        /* Create file compound type */
        dset2_filetype = H5Tcreate(H5T_COMPOUND, sizeof(dset2_t));
        H5Tinsert(dset2_filetype, "a", HOFFSET(dset2_t, a), H5T_STD_I32BE);
        H5Tinsert(dset2_filetype, "b", HOFFSET(dset2_t, b), H5T_IEEE_F32BE);
        H5Tinsert(dset2_filetype, "c", HOFFSET(dset2_t, c), H5T_IEEE_F64BE);

        /* Create memory compound type */
        dset2_memtype = H5Tcreate(H5T_COMPOUND, sizeof(dset2_t));
        H5Tinsert(dset2_memtype, "a", HOFFSET(dset2_t, a), H5T_NATIVE_INT);
        H5Tinsert(dset2_memtype, "b", HOFFSET(dset2_t, b), H5T_NATIVE_FLOAT);
        H5Tinsert(dset2_memtype, "c", HOFFSET(dset2_t, c), H5T_NATIVE_DOUBLE);

        space_id = H5Screate_simple(1, &dim, NULL);
        dset_id = H5Dcreate2(file_id, "dset2", dset2_filetype, space_id,
                             H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite(dset_id, dset2_memtype, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, dset2_data);

        H5Dclose(dset_id);
        H5Sclose(space_id);
        H5Tclose(dset2_filetype);
        H5Tclose(dset2_memtype);
    }

    /*-------------------------------------------------------------------------
     * Create and commit a named datatype "type1" in the root group.
     * This is a compound type with two members:
     *    "a": an array of 4 of H5T_STD_I32BE
     *    "b": a 2D array of [5][6] of H5T_IEEE_F32BE.
     *-----------------------------------------------------------------------*/
    {
        hsize_t dims_a[1] = {4};
        hsize_t dims_b[2] = {5, 6};
        size_t offset = 0, offset_b;
        size_t size_a, size_b, type1_size;

        array_type_a = H5Tarray_create2(H5T_STD_I32BE, 1, dims_a);
        array_type_b = H5Tarray_create2(H5T_IEEE_F32BE, 2, dims_b);

        size_a = H5Tget_size(array_type_a);
        size_b = H5Tget_size(array_type_b);
        type1_size = size_a + size_b;  /* simple concatenation; no extra padding */

        type1 = H5Tcreate(H5T_COMPOUND, type1_size);
        H5Tinsert(type1, "a", offset, array_type_a);
        offset_b = offset + size_a;
        H5Tinsert(type1, "b", offset_b, array_type_b);

        /* Commit the datatype to the file under the name "type1" */
        status = H5Tcommit2(file_id, "type1", type1, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        H5Tclose(array_type_a);
        H5Tclose(array_type_b);
        H5Tclose(type1);
    }

    /*-------------------------------------------------------------------------
     * Create group "group1" and set a comment on it.
     * Then inside group1 create dataset "dset3" whose datatype is the committed
     * type "type1" (opened from the file).
     * Each element of "dset3" is a compound (of type type1_t) with:
     *     a = {0,1,2,3} and
     *     b = a 5x6 array where row i is filled with 0.1*(i+1).
     *-----------------------------------------------------------------------*/
    {
        hsize_t dim = 5;
        type1_t data[5];

        /* Create group "group1" */
        group1_id = H5Gcreate2(file_id, "group1", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        /* Set a comment on group1 */
        status = H5Oset_comment_by_name(file_id, "group1", "This is a comment for group1", H5P_DEFAULT);

        /* Open the committed datatype "type1" */
        dset3_type_id = H5Topen2(file_id, "type1", H5P_DEFAULT);

        space_id = H5Screate_simple(1, &dim, NULL);
        dset_id = H5Dcreate2(group1_id, "dset3", dset3_type_id, space_id,
                             H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        /* Prepare the data: for each element, set a[] = {0,1,2,3} and fill b[][] */
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 4; j++) {
                data[i].a[j] = j;
            }
            for (r = 0; r < 5; r++) {
                for (c = 0; c < 6; c++) {
                    data[i].b[r][c] = 0.1f * (r + 1);
                }
            }
        }
        status = H5Dwrite(dset_id, dset3_type_id, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, data);

        H5Dclose(dset_id);
        H5Sclose(space_id);
        H5Tclose(dset3_type_id);
        H5Gclose(group1_id);
    }

    /*-------------------------------------------------------------------------
     * Create dataset "dset3" (in the root group) that is variable-length.
     * Its datatype is H5T_VLEN of H5T_STD_I32LE and dataspace is 1D with 4 elements.
     * The four data elements are:
     *    (0), (10, 11), (20, 21, 22), (30, 31, 32, 33)
     *-----------------------------------------------------------------------*/
    {
        hsize_t dim = 4;
        hvl_t vdata[4];

        /* Create the vlen datatype */
        vlen_type = H5Tvlen_create(H5T_STD_I32LE);

        space_id = H5Screate_simple(1, &dim, NULL);
        dset_id = H5Dcreate2(file_id, "dset3", vlen_type, space_id,
                             H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        /* Prepare the vlen data */
        /* First element: length 0 (NULL pointer) */
        vdata[0].len = 0;
        vdata[0].p = NULL;
        /* Second element: 2 ints: 10, 11 */
        vdata[1].len = 2;
        vdata[1].p = malloc(2 * sizeof(int));
        ((int *)(vdata[1].p))[0] = 10;
        ((int *)(vdata[1].p))[1] = 11;
        /* Third element: 3 ints: 20, 21, 22 */
        vdata[2].len = 3;
        vdata[2].p = malloc(3 * sizeof(int));
        ((int *)(vdata[2].p))[0] = 20;
        ((int *)(vdata[2].p))[1] = 21;
        ((int *)(vdata[2].p))[2] = 22;
        /* Fourth element: 4 ints: 30, 31, 32, 33 */
        vdata[3].len = 4;
        vdata[3].p = malloc(4 * sizeof(int));
        ((int *)(vdata[3].p))[0] = 30;
        ((int *)(vdata[3].p))[1] = 31;
        ((int *)(vdata[3].p))[2] = 32;
        ((int *)(vdata[3].p))[3] = 33;

        status = H5Dwrite(dset_id, vlen_type, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, vdata);

        /* Free the allocated memory for each vlen element */
        free(vdata[1].p);
        free(vdata[2].p);
        free(vdata[3].p);

        H5Dclose(dset_id);
        H5Sclose(space_id);
        H5Tclose(vlen_type);
    }

    /*-------------------------------------------------------------------------
     * Create group "group2" as a hard link to "/group1".
     *-----------------------------------------------------------------------*/
    status = H5Lcreate_hard(file_id, "group1", file_id, "group2",
                            H5P_DEFAULT, H5P_DEFAULT);

    /*-------------------------------------------------------------------------
     * Create a soft link "slink1" in the root group that points to "somevalue".
     *-----------------------------------------------------------------------*/
    status = H5Lcreate_soft("somevalue", file_id, "slink1",
                            H5P_DEFAULT, H5P_DEFAULT);

    /*-------------------------------------------------------------------------
     * Close file.
     *-----------------------------------------------------------------------*/
    H5Fclose(file_id);

    return 0;
}

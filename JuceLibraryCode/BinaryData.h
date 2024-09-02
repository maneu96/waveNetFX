/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   TrsmUnrolls_inc;
    const int            TrsmUnrolls_incSize = 56496;

    extern const char*   AccelerateSupport;
    const int            AccelerateSupportSize = 2116;

    extern const char*   Cholesky;
    const int            CholeskySize = 1168;

    extern const char*   CholmodSupport;
    const int            CholmodSupportSize = 1958;

    extern const char*   Core;
    const int            CoreSize = 13420;

    extern const char*   Dense;
    const int            DenseSize = 122;

    extern const char*   Eigen;
    const int            EigenSize = 35;

    extern const char*   Eigenvalues;
    const int            EigenvaluesSize = 1836;

    extern const char*   Geometry;
    const int            GeometrySize = 1997;

    extern const char*   Householder;
    const int            HouseholderSize = 887;

    extern const char*   IterativeLinearSolvers;
    const int            IterativeLinearSolversSize = 2160;

    extern const char*   Jacobi;
    const int            JacobiSize = 952;

    extern const char*   KLUSupport;
    const int            KLUSupportSize = 1429;

    extern const char*   LU;
    const int            LUSize = 1276;

    extern const char*   MetisSupport;
    const int            MetisSupportSize = 1048;

    extern const char*   OrderingMethods;
    const int            OrderingMethodsSize = 2510;

    extern const char*   PardisoSupport;
    const int            PardisoSupportSize = 1174;

    extern const char*   PaStiXSupport;
    const int            PaStiXSupportSize = 1809;

    extern const char*   QR;
    const int            QRSize = 1279;

    extern const char*   QtAlignedMalloc;
    const int            QtAlignedMallocSize = 900;

    extern const char*   Sparse;
    const int            SparseSize = 888;

    extern const char*   SparseCholesky;
    const int            SparseCholeskySize = 1294;

    extern const char*   SparseCore;
    const int            SparseCoreSize = 2270;

    extern const char*   SparseLU;
    const int            SparseLUSize = 1824;

    extern const char*   SparseQR;
    const int            SparseQRSize = 1253;

    extern const char*   SPQRSupport;
    const int            SPQRSupportSize = 1251;

    extern const char*   StdDeque;
    const int            StdDequeSize = 855;

    extern const char*   StdList;
    const int            StdListSize = 784;

    extern const char*   StdVector;
    const int            StdVectorSize = 861;

    extern const char*   SuperLUSupport;
    const int            SuperLUSupportSize = 2301;

    extern const char*   SVD;
    const int            SVDSize = 1674;

    extern const char*   ThreadPool;
    const int            ThreadPoolSize = 2137;

    extern const char*   UmfPackSupport;
    const int            UmfPackSupportSize = 1439;

    extern const char*   CMakeLists_txt;
    const int            CMakeLists_txtSize = 28443;

    extern const char*   modules_textClipping;
    const int            modules_textClippingSize = 189;

    extern const char*   batchnorm_tpp;
    const int            batchnorm_tppSize = 3309;

    extern const char*   batchnorm_eigen_tpp;
    const int            batchnorm_eigen_tppSize = 3339;

    extern const char*   batchnorm_xsimd_tpp;
    const int            batchnorm_xsimd_tppSize = 3353;

    extern const char*   batchnorm2d_tpp;
    const int            batchnorm2d_tppSize = 3901;

    extern const char*   batchnorm2d_eigen_tpp;
    const int            batchnorm2d_eigen_tppSize = 3999;

    extern const char*   batchnorm2d_xsimd_tpp;
    const int            batchnorm2d_xsimd_tppSize = 3921;

    extern const char*   conv1d_tpp;
    const int            conv1d_tppSize = 4899;

    extern const char*   conv1d_eigen_tpp;
    const int            conv1d_eigen_tppSize = 3687;

    extern const char*   conv1d_xsimd_tpp;
    const int            conv1d_xsimd_tppSize = 4301;

    extern const char*   conv1d_stateless_tpp;
    const int            conv1d_stateless_tppSize = 3748;

    extern const char*   conv1d_stateless_eigen_tpp;
    const int            conv1d_stateless_eigen_tppSize = 3619;

    extern const char*   conv1d_stateless_xsimd_tpp;
    const int            conv1d_stateless_xsimd_tppSize = 3298;

    extern const char*   conv2d_tpp;
    const int            conv2d_tppSize = 3928;

    extern const char*   conv2d_eigen_tpp;
    const int            conv2d_eigen_tppSize = 3947;

    extern const char*   conv2d_xsimd_tpp;
    const int            conv2d_xsimd_tppSize = 3884;

    extern const char*   gru_tpp;
    const int            gru_tppSize = 10049;

    extern const char*   gru_eigen_tpp;
    const int            gru_eigen_tppSize = 7564;

    extern const char*   gru_xsimd_tpp;
    const int            gru_xsimd_tppSize = 10528;

    extern const char*   lstm_tpp;
    const int            lstm_tppSize = 8300;

    extern const char*   lstm_eigen_tpp;
    const int            lstm_eigen_tppSize = 7946;

    extern const char*   lstm_xsimd_tpp;
    const int            lstm_xsimd_tppSize = 8989;

    extern const char*   README_md;
    const int            README_mdSize = 405;

    extern const char*   CMakeLists_txt2;
    const int            CMakeLists_txt2Size = 1493;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 58;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}

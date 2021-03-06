#include "match_utils.h"

namespace gmmreg {

template <typename T>
void ComputeSquaredDistanceMatrix(const vnl_matrix<T>& A,
                                  const vnl_matrix<T>& B, vnl_matrix<T>& D) {
  int m = A.rows();
  int n = B.rows();
  // asssert(A.cols()==B.cols());
  D.set_size(m, n);
  vnl_vector<T> v_ij;

  #pragma omp for
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      v_ij = A.get_row(i) - B.get_row(j);
      D(i, j) = v_ij.squared_magnitude();
    }
  }
}

template<typename T>
int SelectPoints(const vnl_matrix<T>& pts,
    const std::vector<int>& index, vnl_matrix<T>& selected) {
  int n = index.size();
  int d = pts.cols();
  selected.set_size(n,d);
  for (int i = 0; i < n; ++i) {
    selected.update(pts.extract(1, d, index[i]), i);
  }
  return n;
}

template<typename T>
void FindNearestPairs(const vnl_matrix<T>& dist,
    vnl_matrix<int>& pairs, const T& threshold) {
  int m = dist.rows();
  int n = dist.cols();
  vnl_vector<int> col_selected;
  col_selected.set_size(n);  col_selected.fill(0);
  std::vector<int> row_index, col_index;
  for (int i = 0; i < m; ++i) {
    T min_dist = dist.get_row(i).min_value();
    if (min_dist < threshold) {
      for (int j = 0; j < n; ++j) {
        if (dist(i,j) == min_dist && col_selected[j] == 0){
          row_index.push_back(i);
          col_index.push_back(j);
          col_selected[j] = 1;
        }
      }
    }
  }
  pairs.set_size(2, row_index.size());
  for (int i = 0; i<pairs.cols(); ++i){
    pairs(0,i) = row_index[i];
    pairs(1,i) = col_index[i];
  }
}

template<typename T>
void PickIndices(const vnl_matrix<T>& dist,
    std::vector<int>& row_index, std::vector<int>& col_index,
    const T& threshold) {
  int m = dist.rows();
  int n = dist.cols();
  vnl_vector<int> row_flag, col_flag;
  col_flag.set_size(n);  col_flag.fill(0);
  row_flag.set_size(n);  row_flag.fill(0);
  for (int i = 0; i < m; ++i) {
    T min_dist = dist.get_row(i).min_value();
    if (min_dist < threshold) {
      for (int j = 0; j < n; ++j){
        if (dist(i,j) == min_dist && col_flag[j] == 0){
          row_index.push_back(i);
          row_flag[i] = 1;
          col_index.push_back(j);
          col_flag[j] = 1;
        }
      }
    }
  }
}

template<typename T>
int FindWorkingPairs(const vnl_matrix<T>& M, const vnl_matrix<T>& S,
    const vnl_matrix<T>& Transformed_M, const T & threshold,
    vnl_matrix<T>& working_M, vnl_matrix<T>& working_S) {
  vnl_matrix<T> dist;
  ComputeSquaredDistanceMatrix<T>(Transformed_M, S, dist);
  std::vector<int> row_index, col_index;
  PickIndices<T>(dist, row_index, col_index, threshold);
  SelectPoints<T>(M, row_index, working_M);
  SelectPoints<T>(S, col_index, working_S);
  return row_index.size();
}

}  // namespace gmmreg

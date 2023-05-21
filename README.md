# kmeans
## Introduction
The K-means algorithm is a popular clustering method for finding a partition of N unlabeled
observations into K distinct clusters, where K is a parameter of the method. In this assignment
you will implement this algorithm in both Python and C.
### K-means
Given a set of N datapoints x1, x2, . . . , xN ∈ R^d, the goal is to group the data into K ∈ N
clusters, each datapoint is assigned to exactly one cluster and the number of clusters K is such
that 1 < K < N. Each cluster k is represented by it’s centroid which is the mean µk ∈ Rd of the
cluster’s members.

### k-means clustering algorithm
1. Initialize centroids as first k datapoints: µk = xk, ∀k ∈ K
2. repeat
3. Assign every xi to the closest cluster k: argmin k d(xi, µk), ∀k 1 ≤ k ≤ K
4. Update the centroids: µk =1/|k|*Pxi∈kxi
5. until convergence: (∆µk < eps) OR (iteration_number = iter)

####Where:
• d(p, q) = ((p1 − q1)^2 +...+ (pd − qd)^2)^0.5

• ∆µk: Eclidean Distance, between the updated centroid to the previous one. (this should be checked for every centroid).

### Steps implemented in SIFT

- Intensity of the pixel
- Gradient of the Intensities to know the direction and orientation of the change in state, this helps in detecting corner/edges.
- A Structure tensor from the intensities are calculated which gives us eigenvalues and vectors. Eigenvalues provide insight on whether the region is flat surface, edge or a corner.
- Applying Gaussian blur and creating a scale space, where multiple scale factors are applied to the image.
- Calculate the difference of Gaussians which helps in localising a keypoint. Stack the blurred images and get the 3D local extremum in x,y, and scale from all the 26 neightbours. 9(up image) + 9 (down image) + 8 (central image exculding the central pixel as its the point in the process).
- Keypoint needs to be further refined hence taylor expansion is used and the hessian matrix is calculated to get the true extremum. It uses the eigenvalues directly ((trace)^2/det)
- Orientation needs to be calculated as the image could be rotated later, gradient of orientation is calculated for all the keypoints and a histogram of orientation values is taken and applied to the keypoints
- 

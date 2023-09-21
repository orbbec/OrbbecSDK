#include <stdio.h>
#include <stdlib.h>

#include "utils.hpp"

#include <libobsensor/h/ObTypes.h>
#include <libobsensor/h/Frame.h>
#include <libobsensor/h/Pipeline.h>
#include <libobsensor/h/StreamProfile.h>
#include <libobsensor/h/Error.h>
#include <libobsensor/h/Device.h>
#include <libobsensor/h/Context.h>
#include <libobsensor/h/Filter.h>

#define KEY_ESC 27
#define KEY_R 82
#define KEY_r 114

ob_error    *error    = NULL;  // Used to return SDK interface error information
ob_pipeline *pipeline = NULL;  // pipeline, used to open the Color and Depth streams after connecting the device

void check_error(ob_error *error) {
    if(error) {
        printf("ob_error was raised: \n\tcall: %s(%s)\n", ob_error_function(error), ob_error_args(error));
        printf("\tmessage: %s\n", ob_error_message(error));
        printf("\terror type: %d\n", ob_error_exception_type(error));
        ob_delete_error(error);
        exit(EXIT_FAILURE);
    }
}

// Save point cloud data to ply
void save_points_to_ply(ob_frame *frame, const char *fileName) {
    int pointsSize = ob_frame_data_size(frame, &error) / sizeof(ob_point);
    check_error(error);

    FILE *fp = fopen(fileName, "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "end_header\n");

    ob_point *point = (ob_point *)ob_frame_data(frame, &error);
    check_error(error);
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
        point++;
    }

    fflush(fp);
    fclose(fp);
}

// Save colored point cloud data to ply
void save_rgb_points_to_ply(ob_frame *frame, const char *fileName) {
    int pointsSize = ob_frame_data_size(frame, &error) / sizeof(ob_color_point);
    check_error(error);

    FILE *fp = fopen(fileName, "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "property uchar red\n");
    fprintf(fp, "property uchar green\n");
    fprintf(fp, "property uchar blue\n");
    fprintf(fp, "end_header\n");

    ob_color_point *point = (ob_color_point *)ob_frame_data(frame, &error);
    check_error(error);

    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
        point++;
    }

    fflush(fp);
    fclose(fp);
}

int main(int argc, char **argv) {
    ob_set_logger_severity(OB_LOG_SEVERITY_ERROR, &error);
    check_error(error);

    // Create a pipeline to open the Color and Depth streams after connecting the device
    pipeline = ob_create_pipeline(&error);
    check_error(error);

    // Create config to configure the resolution, frame rate, and format of Color and Depth streams
    ob_config *config = ob_create_config(&error);
    check_error(error);

    // Configure depth flow
    ob_stream_profile      *depth_profile = NULL;
    ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
    check_error(error);
    // Open the default profile of Depth Sensor, which can be configured through the configuration file
    if(profiles) {
        depth_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
        check_error(error);
    }
    ob_config_enable_stream(config, depth_profile, &error);  // enable stream
    check_error(error);

    // Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds
    ob_config_set_align_mode(config, ALIGN_D2C_HW_MODE, &error);
    check_error(error);

    // Configure color flow
    ob_stream_profile *color_profile = NULL;
    profiles                         = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
    if(error) {
        printf("Current device is not support color sensor!\n");
        ob_delete_error(error);
        error = NULL;
        // Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds
        ob_config_set_align_mode(config, ALIGN_DISABLE, &error);
        check_error(error);
    }

    // Open the default profile of Color Sensor, which can be configured through the configuration file
    if(profiles) {
        color_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
    }

    // enable stream
    if(color_profile) {
        ob_config_enable_stream(config, color_profile, &error);
        check_error(error);
    }

    // Get the device handle
    ob_device *device = ob_pipeline_get_device(pipeline, &error);
    check_error(error);

    // Start the pipeline with config
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_error(error);

    // Create a point cloud Filter object (device parameters will be obtained inside the Pipeline when the point cloud filter is created, so try to configure
    // the device before creating the filter)
    ob_filter *point_cloud = ob_create_pointcloud_filter(&error);
    check_error(error);

    // Obtain the current open-stream camera parameters from the pipeline and pass them to the point cloud filter
    ob_camera_param camera_param = ob_pipeline_get_camera_param(pipeline, &error);
    check_error(error);
    ob_pointcloud_filter_set_camera_param(point_cloud, camera_param, &error);
    check_error(error);

    // Operating Tips
    printf("Press R to create rgbd pointCloud and save to ply file!\n");
    printf("Press D to create depth pointCloud and save to ply file!\n");
    printf("Press ESC_KEY to exit!\n");

    int  count          = 0;
    bool points_created = false;
    while(true) {
        if(kbhit()) {
            int key = getch();
            // press esc key to exit
            if(key == KEY_ESC) {
                break;
            }
            if(key == 'R' || key == 'r') {
                count          = 0;
                points_created = false;
                // Generate a colored point cloud and save it
                printf("Save RGBD pointCloud ply file...\n");
                // Limit up to 10 repetitions
                while(count++ < 10) {
                    // Waiting for one frame, the timeout is 100ms
                    ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
                    check_error(error);
                    if(frameset != NULL) {
                        // get depth value scale
                        ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
                        check_error(error);
                        if(depth_frame == NULL) {
                            continue;
                        }

                        // get depth value scale
                        float depth_value_scale = ob_depth_frame_get_value_scale(depth_frame, &error);
                        check_error(error);

                        // delete depth frame
                        ob_delete_frame(depth_frame, &error);
                        check_error(error);

                        // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
                        // millimeter)
                        ob_pointcloud_filter_set_position_data_scale(point_cloud, depth_value_scale, &error);
                        check_error(error);

                        ob_pointcloud_filter_set_point_format(point_cloud, OB_FORMAT_RGB_POINT, &error);
                        check_error(error);
                        ob_frame *pointsFrame = ob_filter_process(point_cloud, frameset, &error);
                        check_error(error);
                        if(pointsFrame != NULL) {
                            save_rgb_points_to_ply(pointsFrame, "rgb_points.ply");
                            printf("rgb_points.ply Saved\n");
                            ob_delete_frame(pointsFrame, &error);
                            check_error(error);
                            points_created = true;
                        }
                        ob_delete_frame(frameset, &error);  // Destroy frameSet to reclaim memory
                        check_error(error);
                        if(points_created) {
                            break;
                        }
                    }
                }
            }
            else if(key == 'D' || key == 'd') {
                count          = 0;
                points_created = false;
                // Generate point cloud and save
                printf("Save depth pointCloud to ply file...\n");
                // Limit up to 10 repetitions
                while(count++ < 10) {
                    // Waiting for one frame, the timeout is 100ms
                    ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
                    check_error(error);
                    if(frameset != NULL) {
                        // get depth
                        ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
                        check_error(error);
                        if(depth_frame == NULL) {
                            continue;
                        }

                        // get depth value scale
                        float depth_value_scale = ob_depth_frame_get_value_scale(depth_frame, &error);
                        check_error(error);

                        // delete depth frame
                        ob_delete_frame(depth_frame, &error);
                        check_error(error);

                        // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
                        // millimeter)
                        ob_pointcloud_filter_set_position_data_scale(point_cloud, depth_value_scale, &error);
                        check_error(error);

                        ob_pointcloud_filter_set_point_format(point_cloud, OB_FORMAT_POINT, &error);
                        check_error(error);
                        ob_frame *pointsFrame = ob_filter_process(point_cloud, frameset, &error);
                        check_error(error);
                        if(pointsFrame != NULL) {
                            save_points_to_ply(pointsFrame, "points.ply");
                            printf("points.ply Saved\n");
                            ob_delete_frame(pointsFrame, &error);
                            check_error(error);
                            points_created = true;
                        }
                        ob_delete_frame(frameset, &error);  // Destroy frameSet to reclaim memory
                        check_error(error);
                        if(points_created) {
                            break;
                        }
                    }
                }
            }
        }

        // Wait for up to 100ms for a frameset in blocking mode.
        ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
        check_error(error);
        if(frameset) {
            ob_delete_frame(frameset, &error);  // Destroy frameSet to reclaim memory
            check_error(error);
        }
    }

    // Destroy point cloud filter
    ob_delete_filter(point_cloud, &error);
    check_error(error);

    // stop pipeline
    ob_pipeline_stop(pipeline, &error);
    check_error(error);

    // destroy pipeline
    ob_delete_pipeline(pipeline, &error);
    check_error(error);

    // destroy config
    ob_delete_config(config, &error);
    check_error(error);

    // destroy profile
    ob_delete_stream_profile(depth_profile, &error);
    check_error(error);

    // destroy profile
    ob_delete_stream_profile(color_profile, &error);
    check_error(error);

    // destroy profile list
    ob_delete_stream_profile_list(profiles, &error);
    check_error(error);

    return 0;
}

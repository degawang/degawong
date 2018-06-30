#include "cImageMerge.h"

namespace degawong {

/* hardmix */
cv::Mat cImageMerge::hardmix() {

    cv::Mat outputMat;
    cv::Mat f_image_1, f_image_2;

    image_1.convertTo(f_image_1, CV_32FC3);
    image_2.convertTo(f_image_2, CV_32FC3);

    cv::Mat_<cv::Vec3b>::iterator itend = image_1.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart = image_2.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3f>::iterator itinput = f_image_1.begin<cv::Vec3f>();
    cv::Mat_<cv::Vec3f>::iterator itaddit = f_image_2.begin<cv::Vec3f>();

    for (; itstart != itend; ++itstart, ++itaddit, ++itinput) {
        for (size_t loop_i = 0; loop_i < image_1.channels(); loop_i++) {
            if (255 > ((*itinput)[loop_i] + (*itaddit)[loop_i])) {
                (*itinput)[loop_i] = 0.0f;
            } else {
                (*itinput)[loop_i] = 255.0f;
            }
        }
    }

    f_image_1.convertTo(outputMat, CV_8UC3);

    return cv::Mat(outputMat);
}

/* overlay */
cv::Mat cImageMerge::overlay() {

    cv::Mat outputMat;
    cv::Mat f_image_1, f_image_2;

    image_1.convertTo(f_image_1, CV_32FC3);
    image_2.convertTo(f_image_2, CV_32FC3);

    //////////////////////////////////////////////////////////////////////////
    // iterator
    cv::Mat_<cv::Vec3b>::iterator itstart = image_2.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend = image_2.end<cv::Vec3b>();

    cv::Mat_<cv::Vec3f>::iterator itinput = f_image_1.begin<cv::Vec3f>();
    cv::Mat_<cv::Vec3f>::iterator itaddit = f_image_2.begin<cv::Vec3f>();

    for (; itstart != itend; ++itstart, ++itaddit, ++itinput) {
        for (size_t loop_i = 0; loop_i < image_1.channels(); loop_i++) {
            if (128 < (*itstart)[loop_i]) {
                (*itinput)[loop_i] = 2 * (*itinput)[loop_i] * (*itaddit)[loop_i] / 255;
            } else {
                (*itinput)[loop_i] = 255 - 2 * (255 - (*itinput)[loop_i]) * (255 - (*itaddit)[loop_i]) / 255;
            }
        }
    }

    f_image_1.convertTo(outputMat, CV_8UC3);

    return cv::Mat(outputMat);
}

/* pinlight */
cv::Mat cImageMerge::pinlight() {

    cv::Mat outputMat;
    cv::Mat f_image_1, f_image_2;

    image_1.convertTo(f_image_1, CV_32FC3);
    image_2.convertTo(f_image_2, CV_32FC3);

    cv::Mat_<cv::Vec3b>::iterator itend = image_1.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart = image_1.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3f>::iterator itinput = f_image_1.begin<cv::Vec3f>();
    cv::Mat_<cv::Vec3f>::iterator itaddit = f_image_2.begin<cv::Vec3f>();

    for (; itstart != itend; ++itstart, ++itaddit, ++itinput) {
        for (size_t loop_i = 0; loop_i < image_1.channels(); loop_i++) {
            if (128 >= (*itstart)[loop_i]) {
                (*itinput)[loop_i] = std::min((*itinput)[loop_i], 2.0f * (*itaddit)[loop_i]);
            } else {
                (*itinput)[loop_i] = std::min((*itinput)[loop_i], 2.0f * (*itaddit)[loop_i] - 255.0f);
            }
        }
    }

    f_image_1.convertTo(outputMat, CV_8UC3);

    return cv::Mat(outputMat);
}

/* hardlight */
cv::Mat cImageMerge::hardlight() {

    cv::Mat outputMat;
    cv::Mat f_image_1, f_image_2;

    image_1.convertTo(f_image_1, CV_32FC3);
    image_2.convertTo(f_image_2, CV_32FC3);

    cv::Mat_<cv::Vec3b>::iterator itstart = image_1.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend = image_1.end<cv::Vec3b>();

    cv::Mat_<cv::Vec3f>::iterator itinput = f_image_1.begin<cv::Vec3f>();
    cv::Mat_<cv::Vec3f>::iterator itaddit = f_image_2.begin<cv::Vec3f>();

    for (; itstart != itend; ++itstart, ++itaddit, ++itinput) {
        for (size_t loop_i = 0; loop_i < image_1.channels(); loop_i++) {
            if (128 < (*itstart)[loop_i]) {
                (*itinput)[loop_i] = 2 * (*itinput)[loop_i] * (*itaddit)[loop_i] / 255;
            } else {
                (*itinput)[loop_i] = 255 - 2 * (255 - (*itinput)[loop_i]) * (255 - (*itaddit)[loop_i]) / 255;
            }
        }
    }

    f_image_1.convertTo(outputMat, CV_8UC3);

    return cv::Mat(outputMat);
}

/* softlight */
cv::Mat cImageMerge::softlight() {

    cv::Mat outputMat;
    cv::Mat f_image_1, f_image_2;

    image_1.convertTo(f_image_1, CV_32FC3);
    image_2.convertTo(f_image_2, CV_32FC3);

    cv::Mat_<cv::Vec3b>::iterator itend = image_2.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart = image_2.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3f>::iterator itinput = f_image_1.begin<cv::Vec3f>();
    cv::Mat_<cv::Vec3f>::iterator itaddit = f_image_2.begin<cv::Vec3f>();

    for (; itstart != itend; ++itstart, ++itaddit, ++itinput) {
        for (size_t loop_i = 0; loop_i < image_1.channels(); loop_i++) {
            if (128 <= (*itstart)[loop_i]) {
                (*itinput)[loop_i] = (*itinput)[loop_i] + ((2 * (*itaddit)[loop_i] - 255.0f) * (*itinput)[loop_i] - ((*itinput)[loop_i] * (*itinput)[loop_i]) / 255.0f) / 255.0f;
            } else {
                (*itinput)[loop_i] = (*itinput)[loop_i] + (2 * (*itaddit)[loop_i] - 255.0f) * (sqrt((*itinput)[loop_i] / 255.0f) * 255.0f - (*itinput)[loop_i]) / 255.0f;
            }
        }
    }

    f_image_1.convertTo(outputMat, CV_8UC3);

    return cv::Mat(outputMat);
}

/* linerlight */
cv::Mat cImageMerge::linerlight() {

    cv::Mat outputMat;
    cv::Mat f_image_1, f_image_2;

    image_1.convertTo(f_image_1, CV_32FC3);
    image_2.convertTo(f_image_2, CV_32FC3);

    //////////////////////////////////////////////////////////////////////////
    // iterator
    cv::Mat_<cv::Vec3b>::iterator itend = image_2.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart = image_2.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3f>::iterator itinput = f_image_1.begin<cv::Vec3f>();
    cv::Mat_<cv::Vec3f>::iterator itaddit = f_image_2.begin<cv::Vec3f>();

    for (; itstart != itend; ++itstart, ++itaddit, ++itinput) {
        for (size_t loop_i = 0; loop_i < image_1.channels(); loop_i++) {
            (*itinput)[loop_i] = std::min(255.0f, std::max(0.0f, ((*itinput)[loop_i] + 2.0f * (*itaddit)[loop_i]) - 255.0f));

        }
    }

    f_image_1.convertTo(outputMat, CV_8UC3);

    return cv::Mat(outputMat);
}

/* vividlight */
cv::Mat cImageMerge::vividlight() {

    cv::Mat outputMat;
    cv::Mat f_image_1, f_image_2;

    image_1.convertTo(f_image_1, CV_32FC3);
    image_2.convertTo(f_image_2, CV_32FC3);

    cv::Mat_<cv::Vec3b>::iterator itend = image_2.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart = image_2.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3f>::iterator itinput = f_image_1.begin<cv::Vec3f>();
    cv::Mat_<cv::Vec3f>::iterator itaddit = f_image_2.begin<cv::Vec3f>();

    for (; itstart != itend; ++itstart, ++itaddit, ++itinput) {
        for (size_t loop_i = 0; loop_i < image_1.channels(); loop_i++) {
            if (128 >= (*itstart)[loop_i]) {
                (*itinput)[loop_i] = std::max(0.0f, ((*itinput)[loop_i] - (255.0f - (*itinput)[loop_i]) * ((255.0f - 2 * (*itaddit)[loop_i])) / (2.0f * (*itaddit)[loop_i])));
            } else {
                (*itinput)[loop_i] = std::min(255.0f, (*itinput)[loop_i] + ((*itinput)[loop_i] * (2.0f * (*itaddit)[loop_i] - 255.0f) / (2.0f * (255.0f - (*itaddit)[loop_i]))));
            }
        }
    }

    f_image_1.convertTo(outputMat, CV_8UC3);

    return cv::Mat(outputMat);
}

}
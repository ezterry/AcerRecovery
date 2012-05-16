$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

$(call inherit-product-if-exists, vendor/acer/picasso_m/picasso_m-vendor.mk)

DEVICE_PACKAGE_OVERLAYS += device/acer/picasso_m/overlay

LOCAL_PATH := device/acer/picasso_m
ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel \
    device/acer/picasso_m/ueventd.picasso_m.rc:root/ueventd.picasso_m.rc

$(call inherit-product, build/target/product/full.mk)

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_NAME := full_picasso_m
PRODUCT_MODEL := A510
PRODUCT_DEVICE := picasso_m
PRODUCT_MANUFACTURER := Acer
PRODUCT_CHARACTERISTICS := tablet

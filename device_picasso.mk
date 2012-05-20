$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

$(call inherit-product-if-exists, vendor/acer/picasso/picasso-vendor.mk)

DEVICE_PACKAGE_OVERLAYS += device/acer/picasso/overlay

PRODUCT_COPY_FILES += \
    device/acer/picasso/ueventd.picasso.rc:root/ueventd.picasso.rc

$(call inherit-product, build/target/product/full.mk)

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_NAME := full_picasso
PRODUCT_MODEL := A500
PRODUCT_DEVICE := picasso
PRODUCT_MANUFACTURER := Acer
PRODUCT_CHARACTERISTICS := tablet

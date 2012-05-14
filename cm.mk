## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := picasso

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/acer/picasso/device_picasso.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := picasso
PRODUCT_NAME := cm_picasso
PRODUCT_BRAND := acer
PRODUCT_MODEL := picasso
PRODUCT_MANUFACTURER := acer

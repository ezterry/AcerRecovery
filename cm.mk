## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := picasso_m

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/acer/picasso_m/device_picasso.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := picasso_m
PRODUCT_NAME := cm_picasso_m
PRODUCT_BRAND := acer
PRODUCT_MODEL := picasso_m
PRODUCT_MANUFACTURER := acer

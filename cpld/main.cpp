#include <sdbusplus/async.hpp>
#include <sdbusplus/server.hpp>
#include "common/include/software_manager.hpp"

// enum Intf {
//   INTF_I2C,
//   INTF_JTAG,
// };

// struct Driver {
//   std::string_view name;
//   uint32_t dev_id;
//   Intf intf;
// };

// static const std::vector<Driver> drivers = {
//   {
//     .name = "LCMXO3-2100C",
//     .dev_id = 0x612BB043,
//     .intf = INTF_I2C,
//     .cpld_open = cpld_dev_open_i2c,
//     .cpld_close = cpld_dev_close_i2c,
//     .cpld_ver = common_cpld_Get_Ver_i2c,
//     .cpld_program = XO2XO3Family_cpld_update_i2c,
//     .cpld_dev_id = common_cpld_Get_id_i2c,
//   },
//   {
//     .name = "LCMXO2-4000HC",
//     .dev_id = 0x012BC043,
//     .intf = INTF_I2C,
//     .cpld_open = cpld_dev_open_i2c,
//     .cpld_close = cpld_dev_close_i2c,
//     .cpld_ver = common_cpld_Get_Ver_i2c,
//     .cpld_program = XO2XO3Family_cpld_update_i2c,
//     .cpld_dev_id = common_cpld_Get_id_i2c,
//   },
//   {
//     .name = "MAX10-10M16",
//     .dev_id = 0x01234567,
//     .intf = INTF_I2C,
//     .cpld_open = cpld_dev_open,
//     .cpld_close = cpld_dev_close,
//     .cpld_ver = max10_get_fw_ver,
//     .cpld_program = max10_cpld_cfm_update,
//     .cpld_dev_id = max10_cpld_get_id,
//   },
// };

struct Cpld : Device {
  int bus;
  int address;

  Cpld(sdbusplus::async::context& ctx,
       SoftwareConfig& config,
       SoftwareManager* parent,
       int bus, int address)
    : Device(ctx, false, config, parent),
      bus(bus),
      address(address)
  {}

  auto updateDevice(const uint8_t* image, size_t image_size, std::unique_ptr<SoftwareActivationProgress>& activationProgress) -> sdbusplus::async::task<bool> {
    (void)image;
    (void)image_size;
    (void)activationProgress;
    co_return true;
  }

  auto getInventoryItemObjectPath() -> sdbusplus::async::task<std::string> {
    co_return "";
  }
};

struct CpldFwManager : SoftwareManager {
  std::vector<Device> devices;

  static constexpr auto configType = "CPLD";

  CpldFwManager(sdbusplus::async::context& ctx)
    : SoftwareManager(ctx, configType, false)
  {}

  auto getInitialConfigurationSingleDevice(
    const std::string& service,
    const std::string& path,
    SoftwareConfig& config
  ) -> sdbusplus::async::task<> {
    auto bus = *co_await SoftwareManager::dbusGetRequiredConfigurationProperty<std::string>(service, path, "Bus", config);
    auto address = *co_await SoftwareManager::dbusGetRequiredConfigurationProperty<std::string>(service, path, "Address", config);
    auto type = *co_await SoftwareManager::dbusGetRequiredConfigurationProperty<std::string>(service, path, "Type", config);
    auto device = std::make_unique<Cpld>(ctx, config, this, std::stoi(bus), std::stoi(address));
    (void)device;
    co_return;
  }
};

int main() {
  auto ctx = sdbusplus::async::context{};
  auto cpld_fw_manager = CpldFwManager(ctx);
  ctx.run();
  return 0;
}

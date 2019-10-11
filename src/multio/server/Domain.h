
#ifndef multio_server_LocalIndices_H
#define multio_server_LocalIndices_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include <eckit/io/Buffer.h>

namespace multio {
namespace server {

class Domain {
public:
    Domain(std::vector<int32_t>&& idx);
    virtual ~Domain() = default;

    virtual void to_global(const std::vector<double>& local, std::vector<double>& global) const = 0;
    virtual void to_global(const eckit::Buffer& local, eckit::Buffer& global) const = 0;

    virtual void to_local(const std::vector<double>& global, std::vector<double>& local) const = 0;
    virtual void to_local(const eckit::Buffer& global, eckit::Buffer& local) const = 0;

protected:
    std::vector<int32_t> indices_;  // Grid-point

};

class Unstructured final : public Domain {
public:
    Unstructured(std::vector<int32_t>&& idx);

private:
    void to_global(const std::vector<double>& local, std::vector<double>& global) const override;
    void to_global(const eckit::Buffer& local, eckit::Buffer& global) const override;

    void to_local(const std::vector<double>& global, std::vector<double>& local) const override;
    void to_local(const eckit::Buffer& global, eckit::Buffer& local) const override;
};

class Structured final : public Domain {
public:
    Structured(std::vector<int32_t>&& idx);

private:
    void to_global(const std::vector<double>& local, std::vector<double>& global) const override;
    void to_global(const eckit::Buffer& local, eckit::Buffer& global) const override;

    void to_local(const std::vector<double>& global, std::vector<double>& local) const override;
    void to_local(const eckit::Buffer& global, eckit::Buffer& local) const override;
};

class Spectral final : public Domain {
public:
    Spectral(std::vector<int32_t>&& idx);

private:
    void to_global(const std::vector<double>& local, std::vector<double>& global) const override;
    void to_global(const eckit::Buffer& local, eckit::Buffer& global) const override;

    void to_local(const std::vector<double>& global, std::vector<double>& local) const override;
    void to_local(const eckit::Buffer& global, eckit::Buffer& local) const override;
};

}  // namespace server
}  // namespace multio

#endif

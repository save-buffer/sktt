#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <tt-metalium/command_queue.hpp>
#include <tt-metalium/constants.hpp>
#include <tt-metalium/host_api.hpp>
#include <tt-metalium/tilize_utils.hpp>
#include <tt-metalium/util.hpp>

namespace ttm = tt::tt_metal;

struct PyMetaliumDevice
{
    PyObject_HEAD
    ttm::IDevice *device;
};

static PyTypeObject PyMetaliumDeviceType =
{
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pymetalium.Device",
    .tp_basicsize = sizeof(PyMetaliumDevice),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_DISALLOW_INSTANTIATION,
    .tp_doc = PyDoc_STR("Handle to a TT-Metalium Device"),
    .tp_new = NULL, // Users should use create_device instead
};

static PyObject *pymetalium_create_device(PyObject *self, PyObject *args)
{
    int device_id;
    if(!PyArg_ParseTuple(args, "i", &device_id))
        return NULL;
    PyMetaliumDevice *result = PyObject_New(PyMetaliumDevice, &PyMetaliumDeviceType);
    try
    {
        result->device = ttm::CreateDevice(device_id);
    }
    catch(const std::exception &e)
    {
        Py_DECREF(result);
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    return (PyObject *)result;
}

extern "C"
{
    static PyModuleDef_Slot pymetalium_module_slots[] =
    {
        { 0, NULL },
    };

    static PyMethodDef pymetalium_methods[] =
    {
        { "create_device", pymetalium_create_device, METH_VARARGS, "Create a device with the given ID" },
        { NULL, NULL, 0, NULL },
    };

    static struct PyModuleDef pymetalium_module =
    {
        .m_base = PyModuleDef_HEAD_INIT,
        .m_name = "pymetalium",
        .m_size = 0,
        .m_methods = pymetalium_methods,
        .m_slots = pymetalium_module_slots,
    };

    PyMODINIT_FUNC PyInit_pymetalium()
    {
        return PyModuleDef_Init(&pymetalium_module);
    }
}

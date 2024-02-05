# Unreal Automation Tool
This automation tool is derived from the Udemy course "Becoming an Unreal Automation Expert". 

The course was initially designed for Unreal Engine 4 but is now outdated. Various bugs have been addressed to ensure compatibility with Unreal Engine 5.3.


## TakeAway
### 1. Asserts in Unreal

#### check
```check``` is the same as assert in C++. It works for Debug, Development, Test, and Shipping Editor builds, not run in shipping builds.

#### verify
```verify``` behaves identically to check. However, it evaluates expressions even in builds where ```check``` macros are disabled. For example, if you have a function that performs an action and then returns a bool indicating whether that action succeeded or failed, you should use Verify rather than Check to make sure that the action was successful. This is because, in shipping builds, Verify will ignore the return value, but will still perform the action. Check, however, will simply not call the function at all in shipping builds, resulting in different behavior.

#### ensure
```ensure``` is similar to ```verify```, but works with non-fatal errors. The Engine will inform the crash reporter but will continue running.


### 2. ```dynamic_cast``` vs ```static_cast```

```dynamic_cast``` is executed at runtime. It is often used for polymorphism downcasting. Ex: UObject to AActor.

```static_cast``` is used for ordinary typecasting. Ex: int to float.

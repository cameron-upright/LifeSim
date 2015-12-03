import bpy
import os
import mathutils
import math

def write_rigid_bodies(f, rigid_bodies, worldQuat, worldScale):

    f.write("rigid_bodies:\n")
    
    for rigid_body in rigid_bodies:

        is_foot = "foot" in rigid_body.keys() and rigid_body["foot"] > 0.0

        location = worldScale * (worldQuat * rigid_body.location)
        rotation = worldQuat * rigid_body.matrix_world.to_quaternion()
        scale    = worldScale * rigid_body.scale

        f.write('  - type: "box"\n')
        f.write('    name: "%s"\n' % rigid_body.name)
        f.write('    position: [%f, %f, %f]\n' % (location.x, location.y, location.z))
        f.write('    rotation: [%f, %f, %f, %f]\n' % (rotation.x, rotation.y, rotation.z, rotation.w))
        f.write('    halfExtents: [%f, %f, %f]\n' % (scale.x, scale.y, scale.z))
        if is_foot:
            f.write('    isFoot: YES\n')
        f.write('\n')

def output_model(armature_name, creature_name, output_name, worldScale, worldQuat):

    # Open the output file
    output_name = '%s/%s' % (os.path.dirname(bpy.data.filepath), output_name)
    f = open(output_name, 'w')

    print(output_name)

    f.write('creature_name: %s\n\n' % creature_name)

    armature = bpy.data.objects[armature_name]
    armature_data = armature.data
    bones = armature.pose.bones

    # Find all the rigid bodies connected to this Armature
    rigid_bodies = []
    bone_rigid_bodies = {}

    for obj in bpy.data.objects:
        if obj.type == 'MESH' and obj.parent == armature and obj.parent_bone != '':
            rigid_bodies.append(obj)
            bone_rigid_bodies[obj.parent_bone] = obj

    write_rigid_bodies(f, rigid_bodies, worldQuat, worldScale)


    f.write("constraints:\n")

    for bone in bones:

        parentBone = bone.parent
        if parentBone == None:
            continue

        rigidBody = bone_rigid_bodies.get(bone.name)
        parentRigidBody = bone_rigid_bodies.get(parentBone.name)

        if rigidBody == None or parentRigidBody == None:
            continue

        axis = []
        axisMin = []
        axisMax = []

        num = 0
        if bone.lock_ik_x == False :
            num += 1
            axis.append(bone.x_axis)
            axisMin.append(bone.ik_min_x)
            axisMax.append(bone.ik_max_x)

        if bone.lock_ik_y == False :
            num += 1
            axis.append(bone.y_axis)
            axisMin.append(bone.ik_min_y)
            axisMax.append(bone.ik_max_y)

        if bone.lock_ik_z == False :
            num += 1
            axis.append(bone.z_axis)
            axisMin.append(bone.ik_min_z)
            axisMax.append(bone.ik_max_z)

        invRotation = rigidBody.matrix_world.to_quaternion().inverted()
        parentInvRotation = parentRigidBody.matrix_world.to_quaternion().inverted()
        pivot = armature.matrix_world*bone.head

        print(bone.name)
        print(num)

        if num == 1 :

            pivotA = worldScale * (invRotation * (pivot - rigidBody.matrix_world.to_translation()))
            pivotB = worldScale * (parentInvRotation * (pivot - parentRigidBody.matrix_world.to_translation()))
            axisA = invRotation * axis[0]
            axisB = parentInvRotation * axis[0]

            print(bone.name)
            print(pivot)
            print(rigidBody.matrix_world.to_translation())
            print(parentRigidBody.matrix_world.to_translation())
            print(axis[0])

            f.write('  - type: hinge\n')
            f.write('    name: "%s"\n' % bone.name)
            f.write('    bodyA: "%s"\n' % rigidBody.name)
            f.write('    bodyB: "%s"\n' % parentRigidBody.name)
            f.write('    axisA: [%f, %f, %f]\n' % (axisA.x, axisA.y, axisA.z))
            f.write('    axisB: [%f, %f, %f]\n' % (axisB.x, axisB.y, axisB.z))
            f.write('    pivotA: [%f, %f, %f]\n' % (pivotA.x, pivotA.y, pivotA.z))
            f.write('    pivotB: [%f, %f, %f]\n' % (pivotB.x, pivotB.y, pivotB.z))
            f.write('    limit: [%f, %f]\n' % (axisMin[0], axisMax[0]))
            f.write('\n');

        elif num == 2 :

            print("--------------------------------------------------")
            print(bone.name)
            print(axis)
            print('    axis2: [%f, %f, %f]\n' % (axis[1].x, axis[1].y, axis[1].z))

            pivotWorld = worldScale * (worldQuat * pivot)
            axisA = worldQuat * axis[0]
            axisB = worldQuat * axis[1]

            f.write('  - type: universal\n')
            f.write('    name: "%s"\n' % bone.name)
            f.write('    bodyA: "%s"\n' % rigidBody.name)
            f.write('    bodyB: "%s"\n' % parentRigidBody.name)
            f.write('    pivot: [%f, %f, %f]\n' % (pivotWorld.x, pivotWorld.y, pivotWorld.z))
            f.write('    axis0: [%f, %f, %f]\n' % (axisA.x, axisA.y, axisA.z))
            f.write('    axis1: [%f, %f, %f]\n' % (axisB.x, axisB.y, axisB.z))
            f.write('    limit0: [%f, %f]\n' % (axisMin[0], axisMax[0]))
            f.write('    limit1: [%f, %f]\n' % (axisMin[1], axisMax[1]))
            f.write('\n');

        else :

            raise Exception('Uhoh')


    f.close()



print()
print()

worldScale = 0.2
worldQuat = mathutils.Quaternion((1,0,0), -math.pi/2)

output_model('Armature', 'Timaaay!', 'creature.yaml', worldScale, worldQuat)

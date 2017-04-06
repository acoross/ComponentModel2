using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shuriken2 : MonoBehaviour {
    public GameObject firePrefab;
    public float speed = 10f;
    public float life = 3f;
    public float actionTime = 0.2f;

    bool firing = false;
    float fireAccumTime = 0f;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void Run() {
        StartCoroutine("Fire2Coroutine");
    }

    IEnumerator Fire2Coroutine() {
        if (firing)
            yield break;

        firing = true;

        {
            var fireInst = GameObject.Instantiate(firePrefab);
            var moveFire = fireInst.GetComponent<MoveFire>();
            moveFire.speed = speed;

            var effect = fireInst.GetComponent<effect>();
            effect.life = life;

            fireInst.transform.position = transform.position;
            fireInst.transform.rotation = transform.rotation;
        }

        {
            var fireInst = GameObject.Instantiate(firePrefab);
            var moveFire = fireInst.GetComponent<MoveFire>();
            moveFire.speed = speed;

            var effect = fireInst.GetComponent<effect>();
            effect.life = life;

            fireInst.transform.position = transform.position;
            fireInst.transform.rotation = transform.rotation;
            fireInst.transform.Rotate(Vector3.forward * -15);
        }

        {
            var fireInst = GameObject.Instantiate(firePrefab);
            var moveFire = fireInst.GetComponent<MoveFire>();
            moveFire.speed = speed;

            var effect = fireInst.GetComponent<effect>();
            effect.life = life;

            fireInst.transform.position = transform.position;
            fireInst.transform.rotation = transform.rotation;
            fireInst.transform.Rotate(Vector3.forward * 15);
        }
        
        yield return new WaitForSeconds(actionTime);

        fireAccumTime = 0f;
        firing = false;
    }
}
